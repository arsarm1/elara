 /* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef WEIGHTED_FAIR_QUEUE_H
#define WEIGHTED_FAIR_QUEUE_H

#include <queue>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/nstime.h"

namespace ns3 {

class TraceContainer;

class WeightedFairQueue: public Queue {
public:
	static TypeId GetTypeId(void);

	WeightedFairQueue();

	~WeightedFairQueue();

	void SetMode(WeightedFairQueue::QueueMode mode);
	WeightedFairQueue::QueueMode GetMode(void);

private:

	virtual bool DoEnqueue(Ptr<Packet> p);
	virtual Ptr<Packet> DoDequeue(void);
	virtual Ptr<const Packet> DoPeek(void) const;

	// returns 0 for 1st queue
	// returns 1 for 2nd queue
	uint16_t Classify(Ptr<Packet> p);

	// collection of finish times of all the packets
	std::map<uint64_t, double_t> getFinishTimeCollection() const;

	// calculates the packet's virtual finish time:
	// finish_time(packet i) = max(finish_time(i-1) , virtual_time(arrival_time(i))) + (packet_size / weight)
	double_t CalculateFinishTime(double_t previousFinishTime, uint32_t packetSize, uint32_t weight);

	// calculates the sum of all active (not empty) queues
	uint32_t CalculateWeightSum(void);

	// approximation of current virtual time
	double_t m_virtualTime;

	// packet classification port
	uint32_t m_secondQueuePort;

	std::queue<Ptr<Packet> > m_firstWeightedQueue;
	uint32_t m_bytesInFirstQueue;
	uint32_t m_firstMaxPackets;
	uint32_t m_firstMaxBytes;
	uint32_t m_firstWeight;

	std::queue<Ptr<Packet> > m_secondWeightedQueue;
	uint32_t m_bytesInSecondQueue;
	uint32_t m_secondMaxPackets;
	uint32_t m_secondMaxBytes;
	uint32_t m_secondWeight;

	QueueMode m_mode;

	// collection of virtual finish times of all the packets enqueued
	std::map<uint64_t, double_t> m_finishTimeCollection;
};

} // namespace ns3

#endif /* WEIGHTED_FAIR_QUEUE_H */

