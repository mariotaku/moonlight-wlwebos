#include "backend/computer_manager.h"

#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <pthread.h>

#include "util/logging.h"

static void *pcmanager_send_wol_action(void *arg);
static bool wol_build_packet(const char *macstr, uint8_t *packet);

bool pcmanager_send_wol(const SERVER_DATA *server)
{
  pthread_t t;
  pthread_create(&t, NULL, pcmanager_send_wol_action, (void *)server->mac);
  return true;
}

void *pcmanager_send_wol_action(void *arg)
{
  uint8_t packet[102];
  if (!wol_build_packet((char *)arg, packet))
  {
    return NULL;
  }
  int broadcast = 1;
  int sockfd;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast))
  {
    applog_e("WoL", "setsockopt() error: %d %s", errno, strerror(errno));
    return NULL;
  }

  struct sockaddr_in client, server;
  client.sin_family = AF_INET;
  client.sin_addr.s_addr = INADDR_ANY;
  client.sin_port = 0;
  // Bind socket
  if (bind(sockfd, (struct sockaddr *)&client, sizeof(client)))
  {
    applog_e("WoL", "bind() error: %d %s", errno, strerror(errno));
    return NULL;
  }

  // Set server endpoint (broadcast)
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("255.255.255.255");
  server.sin_port = htons(9);

  sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&server, sizeof(server));
  if (errno)
  {
    applog_e("WoL", "sendto() error: %d %s", errno, strerror(errno));
    return NULL;
  }
  computer_manager_auto_discovery_schedule(10000);
  return NULL;
}

static bool wol_build_packet(const char *macstr, uint8_t *packet)
{
  unsigned int values[6];
  if (sscanf(macstr, "%x:%x:%x:%x:%x:%x%*c", &values[0], &values[1], &values[2], &values[3], &values[4], &values[5]) != 6)
  {
    return false;
  }
  uint8_t mac[6];
  for (int i = 0; i < 6; i++)
  {
    mac[i] = (uint8_t)values[i];
  }
  memset(packet, 0xFF, 6);
  for (int i = 0; i < 16; i++)
  {
    memcpy(&packet[6 + i * 6], mac, 6);
  }
  return true;
}