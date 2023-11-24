/*****************************************
Contributors:

1 – Yousif Hazim Nazar				2001741
2 - Mohamed Abdelhamid Abdel hares 	2001087
3 – Anas Hamed Ahmed 				2001584
4 – Ahmed Gamal Helmy				2000082
5 – Kirollos George Louis			2000356

****************************************/

// Determines packet size in bytes
#define MAX_PKT 1024

// Boolean type definition
typedef enum { false, true } boolean;

// Sequence or acknowledgment numbers
typedef unsigned int seq_nr;

// Packet definition
typedef struct {
    unsigned char data[MAX_PKT];
} packet;

// Frame kind definition
typedef enum { data_frame, ack_frame, nak_frame } frame_kind;

// Frame structure for transportation
typedef struct {
    frame_kind kind;  // Type of frame
    seq_nr seq;       // Sequence number
    seq_nr ack;       // Acknowledgment number
    packet info;      // Network layer packet
} frame;

// Event types
typedef enum { timeout_event, network_layer_ready_event } event_type;

// Function prototypes

// Wait for an event to occur and return its type
void wait_for_event(event_type *event);

// Fetch a packet from the network layer for transmission
void from_network_layer(packet *p);

// Deliver information from an inbound frame to the network layer
void to_network_layer(packet *p);

// Fetch an inbound frame from the physical layer and copy it to r
void from_physical_layer(frame *r);

// Pass a frame to the physical layer for transmission
void to_physical_layer(frame *s);

// Start the clock and enable the timeout event for a specific sequence number
void start_timer(seq_nr k);

// Stop the clock and disable the timeout event for a specific sequence number
void stop_timer(seq_nr k);

// Start an auxiliary timer and enable the acknowledgment timeout event
void start_ack_timer(void);

// Stop the auxiliary timer and disable the acknowledgment timeout event
void stop_ack_timer(void);

// Allow the network layer to trigger a network layer ready event
void enable_network_layer(void);

// Prevent the network layer from triggering a network layer ready event
void disable_network_layer(void);

// Macro to increment k circularly
#define inc(k) ((k < MAX_SEQ) ? (k = k + 1) : (k = 0))
