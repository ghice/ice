// **********************************************************************
//
// Copyright (c) 2002
// MutableRealms, Inc.
// Huntsville, AL, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef ICE_COLLECTOR_H
#define ICE_COLLECTOR_H

#include <Ice/CollectorF.h>
#include <Ice/InstanceF.h>
#include <Ice/TransceiverF.h>
#include <Ice/AcceptorF.h>
#include <Ice/ThreadPoolF.h>
#include <Ice/ObjectAdapterF.h>
#include <Ice/EndpointF.h>
#include <Ice/TraceLevelsF.h>
#include <Ice/LoggerF.h>
#include <Ice/EventHandler.h>
#include <list>

namespace IceUtil
{

class Exception;

}

namespace Ice
{

typedef IceUtil::Exception LocalException;
class ObjectAdapterI;

}

namespace IceInternal
{

class Incoming;

class Collector : public EventHandler, public JTCRecursiveMutex
{
public:

    void destroy();
    bool destroyed() const;
    void hold();
    void activate();
    void prepareReply(Incoming*);
    void sendReply(Incoming*);

    //
    // Operations from EventHandler
    //
    virtual bool server() const;
    virtual bool readable() const;
    virtual void read(BasicStream&);
    virtual void message(BasicStream&);
    virtual void exception(const ::Ice::LocalException&);
    virtual void finished();
    virtual bool tryDestroy();

private:

    Collector(const InstancePtr&, const ::Ice::ObjectAdapterPtr&, const TransceiverPtr&, const EndpointPtr&);
    virtual ~Collector();
    friend class CollectorFactory;

    enum State
    {
	StateActive,
	StateHolding,
	StateClosing,
	StateClosed
    };

    void setState(State);
    void closeConnection();
    void warning(const ::Ice::LocalException&) const;

    ::Ice::ObjectAdapterPtr _adapter;
    TransceiverPtr _transceiver;
    EndpointPtr _endpoint;
    TraceLevelsPtr _traceLevels;
    ::Ice::LoggerPtr _logger;
    ThreadPoolPtr _threadPool;
    int _responseCount;
    State _state;
};

class CollectorFactory : public EventHandler, public JTCMutex
{
public:

    void destroy();
    void hold();
    void activate();

    EndpointPtr endpoint() const;
    bool equivalent(const EndpointPtr&) const;

    //
    // Operations from EventHandler
    //
    virtual bool server() const;
    virtual bool readable() const;
    virtual void read(BasicStream&);
    virtual void message(BasicStream&);
    virtual void exception(const ::Ice::LocalException&);
    virtual void finished();
    virtual bool tryDestroy();
    
private:

    CollectorFactory(const InstancePtr&, const ::Ice::ObjectAdapterPtr&, const EndpointPtr&);
    virtual ~CollectorFactory();
    friend class ::Ice::ObjectAdapterI;

    enum State
    {
	StateActive,
	StateHolding,
	StateClosing,
	StateClosed
    };

    void setState(State);
    void clearBacklog();
    void warning(const ::Ice::LocalException&) const;

    ::Ice::ObjectAdapterPtr _adapter;
    AcceptorPtr _acceptor;
    TransceiverPtr _transceiver;
    EndpointPtr _endpoint;
    TraceLevelsPtr _traceLevels;
    ::Ice::LoggerPtr _logger;
    ThreadPoolPtr _threadPool;
    std::list<CollectorPtr> _collectors;
    State _state;
};

}

#endif
