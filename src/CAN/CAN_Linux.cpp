#ifdef LINUX

#include "CAN/CAN_Linux.h"

#include <cstdio>
#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>

CAN_Linux::CAN_Linux(const char * ifname)
    : _ifname(ifname), _sock(-1)
{}

CAN_Linux::~CAN_Linux() {
    if (_sock >= 0) {
        close(_sock);
        _sock = -1;
    }
}

bool CAN_Linux::init() {
    _sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (_sock < 0) {
        perror("CAN_Linux: socket()");
        return false;
    }

    struct ifreq ifr;
    strncpy(ifr.ifr_name, _ifname, IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';
    if (ioctl(_sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("CAN_Linux: ioctl(SIOCGIFINDEX)");
        close(_sock);
        _sock = -1;
        return false;
    }

    struct sockaddr_can addr{};
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(_sock, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
        perror("CAN_Linux: bind()");
        close(_sock);
        _sock = -1;
        return false;
    }

    /* Non-blocking receive so tasks don't stall polling. */
    int flags = fcntl(_sock, F_GETFL, 0);
    fcntl(_sock, F_SETFL, flags | O_NONBLOCK);

    return true;
}

bool CAN_Linux::send(CAN_Frame * frame) {
    if (_sock < 0 || frame == nullptr) return false;

    struct can_frame cf{};
    cf.can_id  = frame->id & CAN_SFF_MASK;   /* 11-bit standard ID */
    cf.can_dlc = frame->dlc;
    memcpy(cf.data, frame->data, frame->dlc);

    ssize_t n = write(_sock, &cf, sizeof(cf));
    return n == sizeof(cf);
}

bool CAN_Linux::receive(CAN_Frame * frame) {
    if (_sock < 0 || frame == nullptr) return false;

    struct can_frame cf{};
    ssize_t n = read(_sock, &cf, sizeof(cf));
    if (n != sizeof(cf)) return false;   /* EAGAIN or error */

    frame->id  = cf.can_id & CAN_SFF_MASK;
    frame->dlc = cf.can_dlc;
    memcpy(frame->data, cf.data, cf.can_dlc);
    return true;
}

#endif /* LINUX */
