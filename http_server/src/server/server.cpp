#include <server.hpp>
#include <binary_functions.hpp>


#define SERVER_IP "127:0:0:1"
#define PORT 8080
#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];

void run() 
{
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = sizeof(client_addr);//check
	std::vector<std::thread> threads_array;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
		getchar(); // Pause Console 
        exit(true);
    }

    // Create socket
	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == SOCKET_ERROR || server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
		getchar(); // Pause Console 
        exit(true);
    }

    // Bind socket to address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
	InetPton(AF_INET, SERVER_IP, &server_addr.sin_addr); // Defining The Network Address to Run the Server on
	server_addr.sin_port = htons(PORT); //sorting the port in network byte order and defining it
	
	int sock_result = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)); //the struct return the address after parsing it according to the family and port binding the host to port
    if (sock_result == SOCKET_ERROR) 
        server_error_handler(server_socket, "Bind failed: ");
    

    // Listen for incoming connections
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) 
        server_error_handler(server_socket, "Listen failed: ");
	
    std::cout << "Server listening on " << SERVER_IP << ":" << PORT << std::endl;

    // Accept and handle incoming connections
    while (true) {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size)) == INVALID_SOCKET) 
             server_error_handler(server_socket, "Accept failed: ");
			 
        // Receive client request
        threads_array.push_back(std::thread(client_received_handler, client_socket));
    }

    // Cleanup
    closesocket(server_socket);
    WSACleanup();
}

void server_error_handler(SOCKET server_socket, std::string error_message) {
	std::cerr << error_message << WSAGetLastError() << std::endl;
	closesocket(server_socket);
	WSACleanup();
	getchar(); // Pause Console 
	exit(true);
}

void client_received_handler(SOCKET client_socket) {
	int nDataLength;
	std::string myString;
	std::cout << "in" << "\n"; 

	std::vector<char> image_data;      // Vector to accumulate image data


  // Step 1: Read until the end of headers
    bool headers_received = false;
    size_t header_end_pos = std::string::npos;

    while (!headers_received && (nDataLength = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        myString.append(buffer, nDataLength);

        header_end_pos = myString.find("\r\n\r\n");
        if (header_end_pos != std::string::npos) {
            headers_received = true;
        }
    }

    if (header_end_pos == std::string::npos) {
        std::cerr << "Failed to receive complete headers" << std::endl;
        closesocket(client_socket);
        getchar(); // Pause Console 
	    exit(true);
    }

    // Step 2: Process headers to find Content-Length
    std::string headers = myString.substr(0, header_end_pos + 4); // Include the \r\n\r\n
    size_t content_length = 0;
    std::istringstream header_stream(headers);
    std::string header_line;

    while (std::getline(header_stream, header_line)) {
        if (header_line.find("Content-Length:") != std::string::npos) {
            content_length = std::stoul(header_line.substr(header_line.find(":") + 1));
            break;
        }
    }

    // Step 3: Extract already received body part (if any)
    size_t body_start_pos = header_end_pos + 4;
    if (myString.size() > body_start_pos) {
        image_data.insert(image_data.end(), myString.begin() + body_start_pos, myString.end());
    }

    // Step 4: Read the remaining body content
    while (image_data.size() < content_length && (nDataLength = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        image_data.insert(image_data.end(), buffer, buffer + nDataLength);
    }

    if (nDataLength < 0) {
        std::cerr << "Error receiving data from socket" << std::endl;
    } else if (nDataLength == 0 && image_data.size() < content_length) {
        std::cerr << "Connection closed before receiving the full content" << std::endl;
    }

    //check if it saved it okay using if
    image_data_to_img(image_data);

    // Prepare the HTTP response
    std::string response_body = "<html><body><h1>Hello, World!</h1></body></html>";
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + std::to_string(response_body.size()) + "\r\n";
    response += "Connection: close\r\n";  // Ensure the connection is closed after response
    response += "\r\n";
    response += response_body;

	size_t sizeToSend = response.size();
	size_t sent = 0;

	while(sizeToSend > 0) {
		size_t sentCurrent = send(client_socket, response.c_str() + sent, response.size(), 0);
		std::cout << sentCurrent << "\n"; 
		sizeToSend -= sentCurrent;
		sent += sentCurrent;
	}

	// Close client socket
    closesocket(client_socket);
}
