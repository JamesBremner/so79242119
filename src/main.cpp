#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <climits>
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

struct sHead {
    bool busy;
    int sector;
    bool asc;
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
std::vector<sRequest> theRequestsWaiting;

std::priority_queue<sEvent,std::vector<sEvent>, eventComp> theEventQueue;

sHead theHead;

int theSimTime;

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

sRequest& chooseFastestRequest( int& bestTime )
{
    sRequest& ret = theRequestsWaiting[0];
    bestTime = INT_MAX;
    for( auto& tr : theRequestsWaiting )
    {
        int timeRequired = abs( tr.sector - theHead.sector ) / 5;
        if( timeRequired < bestTime ) {
            bestTime = timeRequired;
            ret = tr;
        }
    }
    return ret;
}

void startRequest()
{
    int timeRequired;
    auto& r = chooseFastestRequest( timeRequired );
    theEventQueue.push( sEvent(r,theSimTime+timeRequired));
    theRequestsWaiting.erase( theRequestsWaiting.begin());
    theHead.busy = true;
    std::cout << "Head moving from " << theHead.sector <<" to "<< r.sector << "\n";
}

void Simulate()
{
    // load request arrivals into event queue
    for( auto & r : theRequests )
        theEventQueue.push(r);
    theSimTime = 0;
    while( theEventQueue.size() )
    {
        auto& e = theEventQueue.top();
        theSimTime = e.time;
        switch( e.type ) {

            case sEvent::eEventType::arrival:
            std::cout << e.request.sector << " sector request arrived at " << theSimTime << "\n";
            theRequestsWaiting.push_back( e.request );
            if( ! theHead.busy)
                startRequest();
            break;

            case sEvent::eEventType::completion:
            std::cout << e.request.sector << " sector completed at " << theSimTime << "\n";
            theHead.sector = e.request.sector;
            theHead.busy = false;
            if( theRequestsWaiting.size() )
                startRequest();
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
