#!/bin/bash

# Function to split the file into chunks
split_file() {
    filename="$1"
    chunk_size=200
    offset=0
    while read -r -n "$chunk_size" chunk; do
        echo "$chunk"
        offset=$((offset + chunk_size))
        dd if="$filename" bs=1 skip="$offset" count="$chunk_size" 2>/dev/null
    done < "$filename"
}

# Array of filenames
filenames=(
    "main.cpp"
    "webserv.hpp"
    "acceptConnectionAndGetRequestData/endOfRequest.cpp"
    "configFile/server.conf"
    "init/initWebStructAndFree.cpp"
    "parseConfigFile/moveBracketsToNextLine.cpp"
    "parseConfigFile/valideServersBlock.cpp"
)

boundary="---------------------------7d3a1d3e043d4"
data=""

# Iterate over the filenames and add chunks to the data
for filename in "${filenames[@]}"; do
    file_content=$(split_file "$filename")

    data+="--$boundary"
    data+=$'\r\nContent-Disposition: form-data; name="file"; filename="'$filename'"'
    data+=$'\r\nContent-Type: text/plain'
    data+=$'\r\n\r\n'
    data+="$file_content"
    data+=$'\r\n'
done

data+="--$boundary--"

# Send the request using curl
curl -X POST \
-H "Transfer-Encoding: chunked" \
-H "Content-Type: multipart/form-data; boundary=$boundary" \
--data-binary "$data" \
http://127.0.0.1:8090

