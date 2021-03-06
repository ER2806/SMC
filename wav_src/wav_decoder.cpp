#include "wav_decoder.h"
#include "string.h"
#include "fstream"
#include "bitset"
#include <iostream>
#include <fstream>

wav_decoder::wav_decoder()
{
}


wav_decoder::~wav_decoder()
{
}


wav_decoder::wav_decoder(const std::string& in_filename)
    : base_decoder(in_filename)
{
}


const std::string wav_decoder::decode()
{
    const int BUFSIZE = 16;
    std::string result_string;
    std::fstream in_stream(input_file, std::ios::in | std::ios::binary);
    if (!in_stream.is_open()){
        throw common_exception("Troubles with opening source wav file.");
    }
    int block_count = 0;
    long message_length;

    while (!in_stream.eof())
    {
        char buff16[BUFSIZE];
        try{
            in_stream.read(buff16, BUFSIZE);
        }
        catch (std::istream::failure&){
            throw common_exception("Troubles with reading from source wav file.");
        }
        if (block_count == 3) // passing the header, encrypt from fitst block
        {
            std::bitset<8> msg;
            std::bitset<8> buffer = buff16[2]; // encrypt one channel
            for (int i = 0; i < 8; i++)
            {
                msg[i] = buffer[i];
            }
            message_length = msg.to_ulong();
        }
        if (block_count < message_length + 5 && block_count > 4)
        {
            std::bitset<8> msg;
            std::bitset<8> buffer = buff16[2];
            for (int i = 0; i < 8; i++)
            {
                msg[i] = buffer[i];
            }
            result_string += msg.to_ulong();
        }
        block_count++;
    }
    std::cout << "result_string = " << result_string << std::endl;
    return result_string;
}
