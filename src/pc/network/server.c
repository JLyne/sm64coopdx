#include "pc/network/network_player.h"
#include "pc/network/packets/packet.h"
#include "pc/debuglog.h"
#include "pc/configfile.h"
#include "pc/network/network.h"

void server_set_max_players(u8 players, bool save) {
    if (gNetworkType != NT_SERVER) {
        LOG_ERROR("Cannot kick a player from a client");
        return;
    }

    if (players < 1 || players > MAX_PLAYERS) {
        LOG_ERROR("refusing to set invalid max player count %d", players);
        return;
    }

    gServerSettings.maxPlayers = players;
    gServerSettings.reservedSlots = MIN(gServerSettings.reservedSlots, gServerSettings.maxPlayers - 1);

    if (save) {
        configAmountOfPlayers = gServerSettings.maxPlayers;
        configReservedSlots = gServerSettings.reservedSlots;
    }

    network_send_server_settings();
}

void server_set_reserved_slots(u8 slots, bool save) {
    if (gNetworkType != NT_SERVER) {
        LOG_ERROR("Cannot kick a player from a client");
        return;
    }

    if (slots > gServerSettings.maxPlayers - 1) {
        LOG_ERROR("refusing to set invalid reserved slot count %d", slots);
        return;
    }

    gServerSettings.reservedSlots = slots;

    if (save) {
        configReservedSlots = gServerSettings.reservedSlots;
    }

    network_send_server_settings();
}

void server_set_reserved_slots_password(const char *password, bool save) {
    if (gNetworkType != NT_SERVER) {
        LOG_ERROR("Cannot kick a player from a client");
        return;
    }

    snprintf(gServerSettings.reservedSlotsPassword, MAX_CONFIG_STRING, "%s", password);

    if (save) {
        snprintf(configReservedSlotsPassword, MAX_CONFIG_STRING, "%s", password);
    }
}

void server_kick_player(struct NetworkPlayer *np, enum KickReasonType reason) {
    if (gNetworkType != NT_SERVER) {
        LOG_ERROR("Cannot kick a player from a client");
        return;
    }

    if (np == NULL)     { return; }
    if (!np->connected) {
        LOG_ERROR("Cannot kick a player who is not connected");
        return;
    }

    if (np->type == NPT_LOCAL) {
        LOG_ERROR("Cannot kick the local player");
        return;
    }

    network_send_kick(np->localIndex, reason);
    network_player_disconnected(np->localIndex);
}
