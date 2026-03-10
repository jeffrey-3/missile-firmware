#ifndef SEEKER_H
#define SEEKER_H

typedef struct {
    float pitch_error;
    float yaw_error;
} seeker_t;

void seeker_init(seeker_t *seeker);
void seeker_update(seeker_t * seeker);

#endif // SEEKER_H
