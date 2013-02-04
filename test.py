import tins

class Handler:
    def __init__(self):
        self.counter = 0
    
    def callback(self, packet):
        return True
    

sniffer = tins.Sniffer("eth0", 2000)
sniffer = tins.FileSniffer("/home/matias/Projects/libtins/benchmark/benchmark01/input.pcap")
handler = Handler()
sniffer.sniff_loop(handler.callback)
#p = x.next_packet()
#print 'Payload type: ' + str(p.pdu().payload_type)
#print 'Source address: ' + str(p.pdu().src_addr)
#print 'Destination address: ' + str(p.pdu().dst_addr)
#p.pdu().dst_addr = '00:0f:ad:fa:de:d0'
#print 'Destination address: ' + str(p.pdu().dst_addr)
#output = ''
#for i in p.pdu().serialize():
    #output += chr(i)
#print repr(output)
