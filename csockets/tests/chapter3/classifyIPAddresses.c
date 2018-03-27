//
// Created by wein on 3/16/18.
//

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <memory.h>

void RunTinyTests();

typedef struct Address {
    unsigned char ip[4];
} Address_t;

typedef struct Classification {
    char token;
    const char *mask;
} Classi_t;

Address_t CreateAddress(
    unsigned char _0,
    unsigned char _1,
    unsigned char _2,
    unsigned char _3) {
    Address_t addr;
    addr.ip[0] = _0;
    addr.ip[1] = _1;
    addr.ip[2] = _2;
    addr.ip[3] = _3;
    return addr;
}

Classi_t Classify(const Address_t *ad) {
    unsigned char msb; // msb: most significant byte
    struct sockaddr_in addr;
    Classi_t cls;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    memcpy(&addr.sin_addr.s_addr, ad->ip, 4);
    msb = *(unsigned char *)&addr.sin_addr.s_addr;
    if ((msb & 0b10000000) == 0) {
        cls.token = 'A';
        cls.mask = "255.0.0.0";
    } else if ((msb & 0b11000000) == 0b10000000) {
        cls.token = 'B';
        cls.mask = "255.255.0.0";
    } else if ((msb & 0b11100000) == 0b11000000) {
        cls.token = 'C';
        cls.mask = "255.255.255.0";
    } else if ((msb & 0b11110000) == 0b11100000) {
        cls.token = 'D';
        cls.mask = "255.255.255.255";
    } else {
        cls.token = 'E';
        cls.mask = "255.255.255.255";
    }
    return cls;
}

void test_givenAddressExpectClassificationToken() {
    Address_t addresses[4] = {
        CreateAddress(44, 135, 86, 12),
        CreateAddress(127, 0, 0, 1),
        CreateAddress(172, 16, 23, 95),
        CreateAddress(192, 168, 9, 1)
    };
    const char classes[4] = {'A', 'A', 'B', 'C'};
    for (int i=0; i<4; ++i) {
        assert(classes[i] == Classify(&addresses[i]).token);
    }
}

void test_givenAddressExpectNetMask() {
    Address_t addresses[4] = {
        CreateAddress(44, 135, 86, 12),
        CreateAddress(127, 0, 0, 1),
        CreateAddress(172, 16, 23, 95),
        CreateAddress(192, 168, 9, 1)
    };
    const char *masks[4] = {
        "255.0.0.0",
        "255.0.0.0",
        "255.255.0.0",
        "255.255.255.0"
    };
    for (int i=0; i<4; ++i) {
        assert(strcmp(masks[i], Classify(&addresses[i]).mask) == 0);
    }
}

/// testing the legacy function inet_addr

void test_givenEmptyCStringExpectNoneAddr() {
    unsigned long addr;
    addr = inet_addr("");
    assert(INADDR_NONE == addr);
}

void test_givenIllegalAddressStringExpectNoneAddr() {
    unsigned long addr;
    addr = inet_addr("127.1.444.12");
    assert(INADDR_NONE == addr);
}

void test_givenAddressStringsExpectAddrs() {
    unsigned long addr;
    addr = inet_addr("192.168.1.23");
    assert(23 == (addr >> 24));
    assert(192 == (addr & 0x000000FF));
}

/// using a better function inet_aton to do the conversion

void test_convertAddressStringExpectAddrStruct() {
    struct sockaddr_in addr;
    int z = 0;
    z = inet_aton("127.12.34.122", &addr.sin_addr);
    assert(0 != z);
    z = inet_aton("127.12.340.122", &addr.sin_addr);
    assert(0 == z);
}

/// round trip conversion inet_aton, inet_ntoa

void test_roundTripConversion() {
    struct sockaddr_in addr;
    int z = 0;
    const char *s;
    z = inet_aton("127.12.34.122", &addr.sin_addr);
    assert(0 != z);
    s = inet_ntoa(addr.sin_addr);
    assert(0 == strcmp("127.12.34.122", s));
}

/// convert IP address from net- to host-order then
/// extract the host portion

const char *ExtractHostID(const char *ad) {
    struct sockaddr_in addr;
    struct sockaddr_in temp;
    in_addr_t hostId;
    if (!inet_aton(ad, &addr.sin_addr)) {
        return 0;
    }
    hostId = inet_lnaof(addr.sin_addr);
    temp.sin_addr = inet_makeaddr(0, hostId);
    return inet_ntoa(temp.sin_addr);
}

const char *ExtractNetID(const char *ad) {
    struct sockaddr_in addr;
    struct sockaddr_in temp;
    in_addr_t netId;
    if (!inet_aton(ad, &addr.sin_addr)) {
        return 0;
    }
    netId = inet_netof(addr.sin_addr);
    temp.sin_addr = inet_makeaddr(netId, 0);
    return inet_ntoa(temp.sin_addr);
}

void test_extractHostID() {
    const char *addrs[4] = {
        "44.135.86.12",
        "127.0.0.1",
        "172.16.23.95",
        "192.168.9.1"
    };
    const char *expected[4] = {
        "0.135.86.12",
        "0.0.0.1",
        "0.0.23.95",
        "0.0.0.1",
    };
    for (int i=0; i<4; ++i) {
        assert(0 == strcmp(expected[i], ExtractHostID(addrs[i])));
    }
}

void test_extractNetId() {
    const char *addrs[4] = {
        "44.135.86.12",
        "127.0.0.1",
        "172.16.23.95",
        "192.168.9.1"
    };
    const char *expected[4] = {
        "44.0.0.0",
        "127.0.0.0",
        "172.16.0.0",
        "192.168.9.0",
    };
    for (int i=0; i<4; ++i) {
        assert(0 == strcmp(expected[i], ExtractNetID(addrs[i])));
    }
}

int main(int argc, char **argv) {
    RunTinyTests();
    return 0;
}