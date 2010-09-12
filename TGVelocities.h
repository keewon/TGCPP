#ifndef TG_VELOCITIES_H
#define TG_VELOCITIES_H

#define TGVelocities_MIN_VELOCITY 15
#define TGVelocities_VELOCITY_INCREMENT 16
#define TGVelocities_PIANO_PIANISSIMO TGVelocities_MIN_VELOCITY
#define TGVelocities_PIANISSIMO (TGVelocities_MIN_VELOCITY + TGVelocities_VELOCITY_INCREMENT)
#define TGVelocities_PIANO (TGVelocities_MIN_VELOCITY + (TGVelocities_VELOCITY_INCREMENT * 2))
#define TGVelocities_MEZZO_PIANO (TGVelocities_MIN_VELOCITY + (TGVelocities_VELOCITY_INCREMENT * 3))
#define TGVelocities_MEZZO_FORTE (TGVelocities_MIN_VELOCITY + (TGVelocities_VELOCITY_INCREMENT * 4))
#define TGVelocities_FORTE (TGVelocities_MIN_VELOCITY + (TGVelocities_VELOCITY_INCREMENT * 5))
#define TGVelocities_FORTISSIMO (TGVelocities_MIN_VELOCITY + (TGVelocities_VELOCITY_INCREMENT * 6))
#define TGVelocities_FORTE_FORTISSIMO (TGVelocities_MIN_VELOCITY + (TGVelocities_VELOCITY_INCREMENT * 7))
#define TGVelocities_DEFAULT TGVelocities_FORTE
    

#endif
