/*#include <libavformat/avformat.h>
#include <SDL2/SDL_mixer.h>
#include <string.h>

typedef struct PacketQueue {
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    SDL_mutex *mutex;
    SDL_cond *cond;
} PacketQueue;

void packet_queue_init(PacketQueue* q) {
    memset(q, 0, sizeof(PacketQueue));
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}

int packet_queue_put(PacketQueue *q, AVPacket *pkt) {

    AVPacketList *pkt1;
    if(av_dup_packet(pkt) < 0) {
        exit_msg("av_dup_packet failed.");
    }

    pkt1 = av_malloc(sizeof(AVPacketList));
    if (!pkt1)
        exit_msg("pkt1 not malloc'd");

    pkt1->pkt = *pkt;
    pkt1->next = NULL;

    SDL_LockMutex()
}*/
