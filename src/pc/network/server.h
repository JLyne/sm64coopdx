#ifndef SERVER_H
#define SERVER_H
#endif

/* |description|Updates the servers's max player count. Must be called from the server.|descriptionEnd| */
void server_set_max_players(u8 players, bool save);
/* |description|Updates the server's reserved slot count. Must be called from the server.|descriptionEnd| */
void server_set_reserved_slots(u8 slots, bool save);
/* |description|Updates the server's reserved slot password. Must be called from the server.|descriptionEnd| */
void server_set_reserved_slots_password(const char *password, bool save);
/* |description|Kicks a player with the given reason. Must be called from the server.|descriptionEnd| */
void server_kick_player(struct NetworkPlayer *np, enum KickReasonType reason);