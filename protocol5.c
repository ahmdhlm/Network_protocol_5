#define MAX_SEQ 7
#include "stdio.h"
#include "protocol.h"

typedef enum {frame_arrival, checksum_err, timeout, network_layer_ready} event_type1;

static boolean is_between(seq_nr a, seq_nr b, seq_nr c) {
    // Determines if a <= b < c in a circular manner
    return (((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a)));
}

static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[]) {
    // Constructs and sends a data frame
    frame s;
    s.info = buffer[frame_nr]; // Insert packet into frame
    s.seq = frame_nr; // Insert sequence number into frame
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1); // Piggyback acknowledgment
    to_physical_layer(&s); // Transmit the frame
    start_timer(frame_nr); // Start the timer
}

void delay()
{
    for(int i=0;i<10000000;i++);
}
void wait_for_event(event_type *event)
{
    event = (event_type *) &event;
}

void from_network_layer(packet *p)
{
    printf("Getting from network layer");
    printf("\n");
    delay();

}

void to_network_layer(packet *p)
{
    printf("Sending to network layer");
    printf("\n");
    delay();
}

void from_physical_layer(frame *r)
{
    printf("Getting from physical layer");
    printf("\n");
    delay();
}

void to_physical_layer(frame *s)
{
    printf("Sending to physical layer");
    printf("\n");
    delay();
}

void start_timer(seq_nr k)
{
    printf("Starting the timer ...");
    for(int i=1;i<50;i++)
    {
        printf("%d seconds passed" , i);
        for(int i=0;i<2000;i++);
    }
}

void stop_timer(seq_nr k)
{
    printf("Stoping the timer ...");
    printf("\n");
    delay();
}

void enable_network_layer(void)
{
    printf("Network layer enabled");
    printf("\n");
    delay();
}

void disable_network_layer(void)
{
    printf("Network layer disabled");
    printf("\n");
    delay();
}

void protocol5(void) {
    seq_nr next_frame_to_send;
    seq_nr ack_expected;
    seq_nr frame_expected;
    frame r;
    packet buffer[MAX_SEQ + 1];
    seq_nr nbuffered;
    seq_nr i;
    event_type event;

    enable_network_layer(); // Allow network layer ready events
    ack_expected = 0;
    next_frame_to_send = 0;
    frame_expected = 0;
    nbuffered = 0;

    while (true) {
        wait_for_event(&event); // Wait for various events

        switch(event) {
            case network_layer_ready:
                // Network layer has a packet to send
                from_network_layer(&buffer[next_frame_to_send]); // Fetch new packet
                nbuffered++;
                send_data(next_frame_to_send, frame_expected, buffer); // Transmit the frame
                inc(next_frame_to_send); // Move sender's window forward
                break;

            case frame_arrival:
                from_physical_layer(&r); // Receive incoming frame

                if (r.seq == frame_expected) {
                    // Accept frames only in order
                    to_network_layer(&r.info); // Pass packet to network layer
                    inc(frame_expected); // Move receiver's window forward
                }

                // Handle piggybacked acknowledgments
                while (is_between(ack_expected, r.ack, next_frame_to_send)) {
                    nbuffered--; // Reduce number of buffered frames
                    stop_timer(ack_expected); // Stop timer for acknowledged frame
                    inc(ack_expected); // Move sender's window forward
                }
                break;

            case checksum_err:
                // Ignore bad frames with checksum errors
                break;

            case timeout:
                // Trouble: retransmit all outstanding frames
                next_frame_to_send = ack_expected; // Start retransmitting here

                for (i = 1; i <= nbuffered; i++) {
                    send_data(next_frame_to_send, frame_expected, buffer); // Resend frame
                    inc(next_frame_to_send); // Prepare to send the next one
                }
                break;
        }

        // Adjust the network layer based on buffered frames
        if (nbuffered < MAX_SEQ) {
            enable_network_layer();
        } else {
            disable_network_layer();
        }
    }
}

int main()
{
    protocol5();
    return 0;
}