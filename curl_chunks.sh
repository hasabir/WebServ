#!/bin/bash

# Function to split the file into chunks of approximately 200 bytes each
split_file() {
    file="$1"
    chunk_size=200
    file_size=$(wc -c < "$file")
    num_chunks=$((file_size / chunk_size))
    if [ $((file_size % chunk_size)) -ne 0 ]; then
        num_chunks=$((num_chunks + 1))
    fi

    dd if="$file" bs="$chunk_size" count="$num_chunks" 2>/dev/null | while read -r -n "$chunk_size" chunk; do
        printf "%x\r\n" "${#chunk}"
        echo "$chunk"
    done
}

# Check if filenames are provided as arguments
if [ $# -lt 1 ]; then
    echo "Please provide the filenames as arguments."
    exit 1
fi

# Execute the cURL command with chunked data
boundary="---------------------------7d3a1d3e043d4"
data=""

for filename in "$@"; do
    file_content=$(split_file "$filename")
    data+="--$boundary\r\n"
    data+="Content-Disposition: form-data; name=\"file\"; filename=\"$filename\"\r\n"
    data+="Content-Type: text/plain\r\n\r\n$file_content\r\n"
done

data+="--$boundary--\r\n"

curl -X POST \
-H "Transfer-Encoding: chunked" \
-H "Content-Type: multipart/form-data; boundary=$boundary" \
--data-binary "$data" \
http://127.0.0.1:8090

