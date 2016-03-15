#ifndef SEMUN_H
#define SEMUN_H
/* $$.It semun$$ */
union semun {				/* define union for semctl() */
  int              val;
  struct semid_ds *buf;
  unsigned short  *array;
};
#endif
