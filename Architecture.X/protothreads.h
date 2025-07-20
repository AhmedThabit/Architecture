#ifndef PROTOTHREADS_H
#define PROTOTHREADS_H

#include "../../pt.h"    // Protothreads core


// -----------------------------------------------------------------------------
// Externally?visible Protothread control blocks
// -----------------------------------------------------------------------------
extern struct pt ptSensor;
extern struct pt ptTelit;
extern struct pt ptEsp32;
extern struct pt ptEth;
extern struct pt ptCLI;


// Forward?declare your threads
PT_THREAD( SensorThread(struct pt *pt) );
PT_THREAD( TelitThread (struct pt *pt) );
PT_THREAD( Esp32Thread(struct pt *pt) );
PT_THREAD( EthThread   (struct pt *pt) );
PT_THREAD( CliThread   (struct pt *pt) );

// Call once before entering main loop
void Protothreads_Init(void);

#endif // PROTOTHREADS_H
