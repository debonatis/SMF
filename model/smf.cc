/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */



#include "ns3/socket-factory.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/simulator.h"

#include "ns3/names.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-routing-table-entry.h"
#include "ns3/ipv4-route.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/udp-socket.h"
#include "ns3/smf.h"
#include "smf.h"

#include<algorithm>




namespace ns3 {

    namespace smf {
        NS_OBJECT_ENSURE_REGISTERED(RoutingProtocol);

        TypeId
        RoutingProtocol::GetTypeId(void) {
            static TypeId tid = TypeId("ns3::smf::RoutingProtocol")
                    .SetParent<Ipv4RoutingProtocol> ()
                    .SetGroupName("Smf")
                    .AddConstructor<RoutingProtocol> ();

            return tid;
        }

        RoutingProtocol::RoutingProtocol() :
        m_ipv4(0)
         {



        }

        RoutingProtocol::~RoutingProtocol() {
        };


        ns3::Ptr<ns3::Ipv4> m_ipv4;
        std::set<uint32_t> m_netdevice;
        std::vector<uint32_t> v;
        Timer m_cleanTimer;
        Time m_cleanIntervall = Time("10s");
        uint32_t iidout;

        void RoutingProtocol::SetNetdevicelistener(std::set<uint32_t> listen) {
            m_netdevice = listen;
        }
        void RoutingProtocol::SetManetIid(uint32_t intout){
            iidout = intout;
        }

        Ptr<Ipv4MulticastRoute>
        RoutingProtocol::LookupStatic(
                Ipv4Address origin,
                Ipv4Address group,
                uint32_t interface, uint8_t ttl) {

            Ptr<Ipv4MulticastRoute> mrtentry = 0;






            mrtentry = Create<Ipv4MulticastRoute> ();
            mrtentry->SetGroup(group);
            mrtentry->SetOrigin(origin);
            mrtentry->SetParent(iidout);
            mrtentry->SetOutputTtl(iidout,ttl);
           


            return mrtentry;
        }

        void
        RoutingProtocol::SetIpv4(Ptr<Ipv4> ipv4) {
            NS_ASSERT(ipv4 != 0);
            NS_ASSERT(m_ipv4 == 0);
            m_cleanTimer.SetFunction(&RoutingProtocol::Clean, this);

            m_ipv4 = ipv4;


        }

        void RoutingProtocol::DoDispose() {
            m_ipv4 = 0;
             m_cleanTimer.Cancel();


        }

        Ptr<Ipv4Route>
        RoutingProtocol::RouteOutput(Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno & sockerr) {
            Ptr<Ipv4Route> rtentry = 0;



            return rtentry;
        }

        bool RoutingProtocol::checkhash(Ptr<const Packet> p) {
            uint32_t hash = Hash32(p->ToString());
            if (std::find(v.begin(), v.end(), hash) == v.end()) {
                v.push_back(hash);
                return true;
            } else {
                return false;
            }

        }

        void RoutingProtocol::Clean() {



            v.erase(v.begin(), v.begin()+(v.size() / 3));
            m_cleanTimer.Schedule(m_cleanIntervall);

        }

        void
        RoutingProtocol::PrintRoutingTable(Ptr<OutputStreamWrapper> stream) const {

        }

        bool RoutingProtocol::RouteInput(Ptr<const Packet> p,
                const Ipv4Header &header, Ptr<const NetDevice> idev,
                UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                LocalDeliverCallback lcb, ErrorCallback ecb) {

            NS_ASSERT(m_ipv4 != 0);
            // Check if input device supports IP 
            NS_ASSERT(m_ipv4->GetInterfaceForDevice(idev) >= 0);
            if (header.GetDestination().IsMulticast()) {
                if (m_ipv4->GetInterfaceForDevice(idev) != (int) iidout) {
                    checkhash(p);
                    return false;
                }

                Ptr<Ipv4MulticastRoute> mrtentry = LookupStatic(header.GetSource(),
                        header.GetDestination(), m_ipv4->GetInterfaceForDevice(idev), header.GetTtl());

                if (mrtentry && checkhash(p)) {





                    mcb(mrtentry, p, header);




                    return true;

                } else {


                    return false;
                }
            }
            return false;
        }

        void
        RoutingProtocol::NotifyInterfaceUp(uint32_t i) {
        }

        void
        RoutingProtocol::NotifyInterfaceDown(uint32_t i) {
        }

        void
        RoutingProtocol::NotifyAddAddress(uint32_t interface, Ipv4InterfaceAddress address) {
        }

        void
        RoutingProtocol::NotifyRemoveAddress(uint32_t interface, Ipv4InterfaceAddress address) {
        }

        void RoutingProtocol::DoInitialize() {


        }







    }

}






