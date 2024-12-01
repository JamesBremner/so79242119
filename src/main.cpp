#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct sRequest
{
    int sector;
    int arrival;
};

struct sEvent
{
    enum class eEventType {
        arrival,
        completion
    };
    eEventType type;
    sRequest request;
    int time;
    sEvent( const sRequest& r )
    : request( r ), time( r.arrival ),
    type( eEventType::arrival)
    {}
    sEvent( const sRequest& r, int t )
    : request( r ), time( t ), type( eEventType::completion)
    {}
};

class eventComp
{
public:

  bool operator() (const sEvent& lhs, const sEvent& rhs) const
  {
     return (lhs.time>rhs.time);
  }
};

std::vector<sRequest> theRequests;

std::priority_queue<sEvent,std::vector<sEvent>, eventComp> theEventQueue;

void readfile(const std::string &fname)
{
    // The input will be of the following format with the first number representing the sector to be serviced
    // and the second representing the arrival time (sorted by arrival time asc):

    std::ifstream ifs(fname);
    if (!ifs.is_open())
        throw std::runtime_error(
            "Cannot open " + fname);
    sRequest R;
    ifs >> R.sector;
    ifs >> R.arrival;
    while (!ifs.fail())
    {
        theRequests.push_back(R);
        ifs >> R.sector;
        ifs >> R.arrival;
    }
}

void Simulate()
{
    // load request arrivals into event queue
    for( auto & r : theRequests )
        theEventQueue.push(r);
    int time = 0;
    while( theEventQueue.size() )
    {
        auto& e = theEventQueue.top();
        time = e.time;
        switch( e.type ) {
            case sEvent::eEventType::arrival:
            std::cout << e.request.sector << " sector request at " << time << "\n";
            theEventQueue.push(
                sEvent( e.request, time + 20 )            );
            break;
            case sEvent::eEventType::completion:
            std::cout << e.request.sector << " sector completed at " << time << "\n";
            break;
        }
        theEventQueue.pop();
    }


}
main()
{
    readfile("../dat/test1.txt");

    Simulate();

    return 0;
}
