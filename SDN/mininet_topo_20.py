import random
import threading
import os
import socket

from mininet.net import Mininet
from mininet.node import Controller, RemoteController, OVSKernelSwitch, UserSwitch, OVSSwitch, Host
from mininet.cli import CLI
from mininet.log import setLogLevel
from mininet.link import TCLink, Link
from mininet.topo import Topo

class Network(Topo):

    def __init__(self):
        Topo.__init__(self)
        h0 = self.addHost('h0')
        s2 = self.addSwitch('s2')
        h1 = self.addHost('h1')
        s3 = self.addSwitch('s3')
        s4 = self.addSwitch('s4')
        s17 = self.addSwitch('s17')
        s5 = self.addSwitch('s5')
        s11 = self.addSwitch('s11')
        s6 = self.addSwitch('s6')
        s16 = self.addSwitch('s16')
        h7 = self.addHost('h7')
        s8 = self.addSwitch('s8')
        h9 = self.addHost('h9')
        h10 = self.addHost('h10')
        s12 = self.addSwitch('s12')
        h13 = self.addHost('h13')
        s14 = self.addSwitch('s14')
        h15 = self.addHost('h15')
        s18 = self.addSwitch('s18')
        h19 = self.addHost('h19')
        self.addLink(h0, s2, cls=TCLink, delay='1ms')
        self.addLink(h1, s2, cls=TCLink, delay='1ms')
        self.addLink(s2, s3, cls=TCLink, delay='1ms')
        self.addLink(s3, s4, cls=TCLink, delay='5ms')
        self.addLink(s3, s17, cls=TCLink, delay='1ms')
        self.addLink(s4, s5, cls=TCLink, delay='1ms')
        self.addLink(s4, s11, cls=TCLink, delay='1ms')
        self.addLink(s5, s6, cls=TCLink, delay='3ms')
        self.addLink(s5, s16, cls=TCLink, delay='1ms')
        self.addLink(s6, h7, cls=TCLink, delay='1ms')
        self.addLink(s6, s8, cls=TCLink, delay='5ms')
        self.addLink(s8, h9, cls=TCLink, delay='1ms')
        self.addLink(s8, h10, cls=TCLink, delay='1ms')
        self.addLink(s11, s12, cls=TCLink, delay='5ms')
        self.addLink(s12, h13, cls=TCLink, delay='1ms')
        self.addLink(s12, s14, cls=TCLink, delay='1ms')
        self.addLink(h15, s17, cls=TCLink, delay='1ms')
        self.addLink(s16, s18, cls=TCLink, delay='1ms')
        self.addLink(s17, h19, cls=TCLink, delay='1ms')
        self.addLink(s2, s4, cls=TCLink, delay='3ms')
        self.addLink(s2, s11, cls=TCLink, delay='1ms')
        self.addLink(s17, s16, cls=TCLink, delay='3ms')
        self.addLink(s5, s17, cls=TCLink, delay='1ms')
        self.addLink(s8, s16, cls=TCLink, delay='1ms')

topos = {
        'mytopo': (lambda: Network())
}
