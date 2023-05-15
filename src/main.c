#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

void scan_greet_print(const int* sock, inquiry_info** info, const int peers) {
	printf("Greetings %d devices\n", peers);

	for (int i=0; i< peers; i++) {
		char peer_addr[36];
		char peer_name[249];
		inquiry_info* peer_device = *info + i;
		ba2str(&(peer_device->bdaddr), peer_addr);

		if (hci_read_remote_name(*sock, &(peer_device->bdaddr), sizeof(peer_name), peer_name, 0) < 0) {
			strcpy(peer_name, "zombie");
		}

		printf("[Device %d] %s %s\n", i, peer_addr, peer_name);
	}
}

int scan_inquiry(const int device, inquiry_info** info, int maxp) {
	const int flags = IREQ_CACHE_FLUSH;
	const int peers = hci_inquiry(device, 3, maxp, NULL, info, flags);
	if (peers < 0) {
		perror("Failed to scan for devices");
		return -1;
	}

	return peers;
}

int main(int argc, char** argv) {
	const int device = hci_get_route(NULL);
	if (device < 0) {
		printf("Error finding bluetooth device\n");
		exit(1);
	}
	
	const int maxp = 12;
	inquiry_info* info = (inquiry_info*) malloc(sizeof(inquiry_info) * maxp);
	const int found = scan_inquiry(device, &info, maxp);
	if (found < 0) {
		printf("Nothing was found\n");
		goto end;
	}

	int sock = hci_open_dev(device);
	scan_greet_print(&sock, &info, found);

end:
	close(sock);
	free(info);

	return 0;
}
