#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct {
   int k;
   int player1_score;
   int player2_score;
   int experiments;
   int thread_count;
   int player1_wins;
   int player2_wins;
   pthread_mutex_t mutex;
} GameData;

int roll_dice() { return (rand() % 6 + 1) + (rand() % 6 + 1); }

void* simulate_games(void* arg) {
   GameData* data = (GameData*)arg;
   int local_player1_wins = 0;
   int local_player2_wins = 0;

   for (int i = 0; i < data->experiments / data->thread_count; i++) {
      int player1_total = data->player1_score;
      int player2_total = data->player2_score;

      for (int j = 0; j < data->k; j++) {
         player1_total += roll_dice();
         player2_total += roll_dice();
      }

      if (player1_total > player2_total) {
         local_player1_wins++;
      } else if (player2_total > player1_total) {
         local_player2_wins++;
      }
   }

   pthread_mutex_lock(&data->mutex);
   data->player1_wins += local_player1_wins;
   data->player2_wins += local_player2_wins;
   pthread_mutex_unlock(&data->mutex);

   return NULL;
}

int main(int argc, char* argv[]) {
   if (argc < 6) {
      fprintf(stderr,
              "Usage: %s <K> <current round> <player1 score> <player2 score> "
              "<experiments> <max threads>\n",
              argv[0]);
      return 1;
   }

   int k = atoi(argv[1]);
   int current_round = atoi(argv[2]);
   int player1_score = atoi(argv[3]);
   int player2_score = atoi(argv[4]);
   int experiments = atoi(argv[5]);
   int max_threads = atoi(argv[6]);

   if (max_threads < 1 || experiments < 1) {
      fprintf(stderr, "Wrong args\n");
      return 1;
   }

   GameData data;
   data.k = k;
   data.player1_score = player1_score;
   data.player2_score = player2_score;
   data.experiments = experiments;
   data.thread_count = max_threads;
   data.player1_wins = 0;
   data.player2_wins = 0;
   pthread_mutex_init(&data.mutex, NULL);

   pthread_t threads[max_threads];
   srand(time(NULL));

   for (int i = 0; i < max_threads; i++) {
      pthread_create(&threads[i], NULL, simulate_games, &data);
   }

   for (int i = 0; i < max_threads; i++) {
      pthread_join(threads[i], NULL);
   }

   printf("Игрок 1 победил в %d эксперимент(ов)\n", data.player1_wins);
   printf("Игрок 2 победил в %d эксперимент(ов)\n", data.player2_wins);

   double total_experiments = data.player1_wins + data.player2_wins;
   printf("Шанс на победу игрока 1: %.2f%%\n",
          (data.player1_wins / total_experiments) * 100);
   printf("Шанс на победу игрока 2: %.2f%%\n",
          (data.player2_wins / total_experiments) * 100);

   pthread_mutex_destroy(&data.mutex);
   return 0;
}
