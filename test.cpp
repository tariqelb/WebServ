void Client::ParseChunked(const char *str, size_t size_read)
{
    std::fstream requestBody;
    const char *buffer = str;
    size_t buffer_size = size_read;

    requestBody.open("bodyRequest" + FileExtension(), std::ios::out | std::ios::app | std::ios::binary);
    while (buffer_size > 0)
    {
        size_t pos = 0;
        std::string token;
        while ((pos = FindStringInBuffer(buffer, buffer_size, "\r\n", 0)) != std::string::npos)
        {
            token = std::string(buffer, pos);
            if (isHexadecimal(token))
            {
                if (token == "0")
                {
                    requestBody.close();
                    set_has_request(true);
                    return;
                }
                buffer += pos + 2;
                buffer_size -= pos + 2;
                pos = 0;
                continue;
            }
            else
            {
                size_t pos2 = FindStringInBuffer(buffer, buffer_size, "\r\n", pos + 2);
                if (pos2 != std::string::npos)
                {
                    std::string nextToken = std::string(buffer + pos + 2, pos2 - pos - 2);
                    if (!isHexadecimal(nextToken))
                    {
                        token.append("\r\n");
                    }
                }
                else
                {
                    token.append("\r\n");
                }
            }
            requestBody.write(token.c_str(), token.size());
            buffer += pos + 2;
            buffer_size -= pos + 2;
            pos = 0;
        }
        requestBody.write(buffer, buffer_size);
        buffer_size = 0;
    }
    // move file pointer to the end of the file
    requestBody.seekg(0, std::ios::end);
    // get current position of file pointer, which is the file size
    lenghtBodyFile = requestBody.tellg();
    setlenghtBodyFile(lenghtBodyFile);
    requestBody.seekg(0, std::ios::beg);
    requestBody.close();
}