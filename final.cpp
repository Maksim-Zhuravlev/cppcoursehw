#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <ctype.h>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "server3/server.hpp"

using boost::asio::ip::tcp;

using namespace std;


int main(int argc, char* argv[])
{
  try
  {
    int index;
    int c;

    char* host = "127.0.0.1";
    char* port = "80";
    char* dir  = "/home/box";

    std::size_t num_threads = 4;

    opterr = 0;

    while ((c = getopt (argc, argv, "h:p:d:")) != -1) {
        switch (c)
          {
          case 'h':
            host = optarg;
            break;
          case 'p':
            port = optarg;
            break;
          case 'd':
            dir = optarg;
            break;
          case '?':
            if (optopt == 'c')
              fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
              fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
              fprintf (stderr,
                       "Unknown option character `\\x%x'.\n",
                       optopt);
            return 1;
          default:
            abort ();
          }
    }


    daemon(0, 0);
    http::server3::server s(host, port, dir, num_threads);    
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
