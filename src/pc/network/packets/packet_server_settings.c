#include <stdio.h>
#include "pc/network/network.h"
#include "pc/debuglog.h"

#include <string.h>

void network_send_server_settings(void) {
    SOFT_ASSERT(gNetworkType == NT_SERVER);

    struct Packet p = { 0 };
    packet_init(&p, PACKET_SERVER_SETTINGS, true, PLMT_NONE);
    packet_write(&p, &gServerSettings.playerInteractions, sizeof(u8));
    packet_write(&p, &gServerSettings.bouncyLevelBounds, sizeof(u8));
    packet_write(&p, &gServerSettings.playerKnockbackStrength, sizeof(u8));
    packet_write(&p, &gServerSettings.stayInLevelAfterStar, sizeof(u8));
    packet_write(&p, &gServerSettings.skipIntro, sizeof(u8));
    packet_write(&p, &gServerSettings.bubbleDeath, sizeof(u8));
    packet_write(&p, &gServerSettings.headlessServer, sizeof(u8));
    packet_write(&p, &gServerSettings.nametags, sizeof(u8));
    packet_write(&p, &gServerSettings.maxPlayers, sizeof(u8));
    packet_write(&p, &gServerSettings.reservedSlots, sizeof(u8));
    packet_write(&p, &gServerSettings.pauseAnywhere, sizeof(u8));
    packet_write(&p, &gServerSettings.pvpType, sizeof(u8));

    network_send(&p);
    LOG_INFO("sending server settings packet");
}

void network_receive_server_settings(struct Packet* p) {
    SOFT_ASSERT(gNetworkType == NT_CLIENT);
    LOG_INFO("received server settings packet");

    packet_read(p, &gServerSettings.playerInteractions, sizeof(u8));
    packet_read(p, &gServerSettings.bouncyLevelBounds, sizeof(u8));
    packet_read(p, &gServerSettings.playerKnockbackStrength, sizeof(u8));
    packet_read(p, &gServerSettings.stayInLevelAfterStar, sizeof(u8));
    packet_read(p, &gServerSettings.skipIntro, sizeof(u8));
    packet_read(p, &gServerSettings.bubbleDeath, sizeof(u8));
    packet_read(p, &gServerSettings.headlessServer, sizeof(u8));
    packet_read(p, &gServerSettings.nametags, sizeof(u8));
    packet_read(p, &gServerSettings.maxPlayers, sizeof(u8));
    packet_read(p, &gServerSettings.reservedSlots, sizeof(u8));
    packet_read(p, &gServerSettings.pauseAnywhere, sizeof(u8));
    packet_read(p, &gServerSettings.pvpType, sizeof(u8));

    // Password isn't sent to client so any value already stored here is incorrect
    gServerSettings.reservedSlotsPassword[0] = 0;

    if (
        p->error
        || gServerSettings.maxPlayers < 1
        || gServerSettings.maxPlayers > MAX_PLAYERS
        || gServerSettings.maxPlayers < gServerSettings.reservedSlots
        || gServerSettings.reservedSlots > MAX_PLAYERS
    ) {
        LOG_ERROR(
            "invalid server settings packet: maxPlayers=%u reservedSlots=%u error=%u",
            gServerSettings.maxPlayers,
            gServerSettings.reservedSlots,
            p->error
        );
        network_shutdown(true, false, false, false);
    }
}
