#include <glog/logging.h>

/**
 * The entry point for the game server.
 * @param argc  The number of arguments.
 * @param argv  The command-line argument values.
 * @return      The return code.
 */
int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "hey";
    return 0;
}