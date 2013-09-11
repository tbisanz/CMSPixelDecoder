#include "CMSPixelDecoder.h"

#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;
using namespace CMSPixel;

int main(int argc, char* argv[]) {

  Log::ReportingLevel() = Log::FromString(argv[1] ? argv[1] : "SUMMARY");

  CMSPixelStatistics global_statistics;
  std::vector<pixel> * evt = new std::vector<pixel>;
  int64_t timestamp = 0;
  int64_t old_timestamp = 0;
  int flags = 0; //FLAG_OLD_RAL_FORMAT;

  int events = atoi(argv[2]);
  std::cout << "Decoding " << events << " events." << std::endl;

  for (int i = 3; i < argc; ++i) {
    std::cout << "Trying to decode " << argv[i] << std::endl;
    CMSPixelFileDecoder * decoder = new CMSPixelFileDecoderRAL(argv[i],8,flags,ROC_PSI46DIGV2);

    for(int j = 0; j < events; ++j) {
      if(decoder->get_event(evt, timestamp) <= DEC_ERROR_NO_MORE_DATA) break;

      if(timestamp < old_timestamp) {
	LOG(logWARNING) << "Timestamps not monotonically increasing!";
      }
      old_timestamp = timestamp;

    }

    global_statistics.update(decoder->statistics);
    decoder->statistics.print();
    delete decoder;
  }

  //  global_statistics.print();
  return 0;
}
