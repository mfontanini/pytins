import tins_core
from tins_core import NetworkInterface, HWAddress6, IPv4Address, IPv6Address, \
ICMPFlags, PacketSender, PacketWriter, FileSniffer, Sniffer, LinkType, \
RawPDU, PDUType, PDUNotFound, NoInnerPDU

def find_pdu(self, pdu_class):
    return self.find_pdu_by_type(pdu_class.pdu_type)

tins_core.PDU.find_pdu = find_pdu

class EthernetII(tins_core.EthernetII):
    def __init__(self, iface = NetworkInterface(), dst_addr = HWAddress6(), src_addr = HWAddress6()):
        tins_core.EthernetII.__init__(self, iface, dst_addr, src_addr)

class IP(tins_core.IP):
    def __init__(self, dst_addr = IPv4Address(), src_addr = IPv4Address()):
        tins_core.IP.__init__(self, dst_addr, src_addr)

class IPv6(tins_core.IPv6):
    def __init__(self, dst_addr = IPv6Address(), src_addr = IPv6Address()):
        tins_core.IPv6.__init__(self, dst_addr, src_addr)

class ARP(tins_core.ARP):
    def __init__(self, target_ip = IPv4Address(), sender_ip = IPv4Address(), target_hw = HWAddress6(), sender_hw = HWAddress6()):
        tins_core.ARP.__init__(self, target_ip, sender_ip, target_hw, sender_hw)
        
class TCP(tins_core.TCP):
    def __init__(self, dport = 0, sport = 0):
        tins_core.TCP.__init__(self, dport, sport)

class UDP(tins_core.UDP):
    def __init__(self, dport = 0, sport = 0):
        tins_core.UDP.__init__(self, dport, sport)

class ICMP(tins_core.ICMP):
    def __init__(self, type=ICMPFlags.ECHO_REQUEST, code=0, check=0, id=0, mtu=0, pointer=0, sequence=0, gateway=0):
        tins_core.ICMP.__init__(self, type)
        if code != 0:
            self.code = code
        if check != 0:
            self.check = check
        if id != 0:
            self.id = id
        if mtu != 0:
            self.mtu = mtu
        if pointer != 0:
            self.pointer = pointer
        if sequence != 0:
            self.sequence = sequence
        if gateway != 0:
            self.gateway = gateway
