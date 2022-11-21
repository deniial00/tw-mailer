#include "ServerController.hpp"

ServerController::ServerController(int port)
{
    std::cout << "Creating server ...";

    int opt = 1;

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(port);

    // Creating socket file descriptor
    if ((this->serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (bind(this->serverSocket, (struct sockaddr*)&this->address, sizeof(this->address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "ok" << std::endl;
}

ServerController::ServerController(int port, std::string baseDir) 
: ServerController::ServerController(port)
{
    this->baseDir = baseDir;
}

void ServerController::Listen()
{
    int client;
    int addrSize = sizeof(this->address);
    pid_t childpid;

    this->isRunning = true;

    // Listening for connections (upto 5)
    if (listen(this->serverSocket, 5) == 0) {
        printf("Listening...\n\n");
    }
    
    int cnt = 0;
    while(this->isRunning)
    {
        client = accept(this->serverSocket, (struct sockaddr*)&this->address, (socklen_t*)&addrSize);

        if (client < 0) {
            exit(1);
        }
        // Displaying information of
        // connected client
        printf("Connection accepted from %s:%d\n",
               inet_ntoa(address.sin_addr),
               ntohs(address.sin_port));
 
        // Print number of clients
        // connected till now
        printf("Clients connected: %d\n\n",
               ++cnt);


        if ((childpid = fork()) == 0) {
 
            // read from socket
            std::string response = this->HandleRequest(client);


            // write to socket
            std::cout   << "Response to client:" << std::endl
                        << response << std::endl << std::endl;

            send(client, response.c_str(), response.length(),0);
        }

        // read from socket
        /*std::string response = this->HandleRequest(client);


        // write to socket
        std::cout   << "Response to client:" << std::endl
                    << response << std::endl << std::endl;

        send(client, response.c_str(), response.length(),0);*/
        
        close(client);
    }
}
 
std::string ServerController::HandleRequest(int client)
{
    char buffer[1024] = {0};

    std::string header, body, response = "";
    // int receivedBytes = 0;

    // TODO: umbau auf timeout after MAX_SEC?
    read(client, buffer, 1024);
    std::string request(buffer,strlen(buffer));
    std::cout   << std::endl << "==================" << std::endl << std::endl << "Request from client:" << std::endl
                << request << std::endl << std::endl;

    try {
        header = request.substr(0, request.find('\n'));
        body = request.substr(request.find("\n") + 1, std::string::npos);
    } catch (...) {
        std::cout << "[ERROR] Could not parse message";
        return "ERR\n";
    }

    if(header == "LOGIN"){
        std::string temp_username = body.substr(0, body.find('\n'));
        std::cout << temp_username << std::endl;
        std::string temp_password = body.substr(body.find("\n") + 1);
        temp_password.pop_back();
        std::cout << temp_password << std::endl;
        std::string base_dn_string = "ou=people,dc=technikum-wien,dc=at";
        std::string temp = ("uid=" + temp_username + "," + base_dn_string);

        LDAP *ldap;
        LDAPMessage *answer, *entry;
        BerElement *ber;

        int  result;
        int  auth_method        = LDAP_AUTH_SIMPLE;
        int  ldap_version       = LDAP_VERSION3;
        const char *ldap_host   = "ldap.technikum.wien.at";
        const char *ldapUri     = "ldap://ldap.technikum-wien.at:389";
        const char *ldap_dn     = temp.c_str();
        const char *ldap_pw     = temp_password.c_str();
        int   ldap_port         = 389;
        const int ldapVersion   = LDAP_VERSION3;

        


        // search settings
        const char *ldapSearchBaseDomainComponent = "dc=technikum-wien,dc=at";
        const char *ldapSearchFilter = "(uid=if21b045)";
        ber_int_t ldapSearchScope = LDAP_SCOPE_SUBTREE;
        const char *ldapSearchResultAttributes[] = {"uid", "cn", NULL};


    int rc = 0; // return code
   // setup LDAP connection
   LDAP *ldapHandle;
   rc = ldap_initialize(&ldapHandle, ldapUri);
   if (rc != LDAP_SUCCESS)
   {
      fprintf(stderr, "ldap_init failed\n");
      exit(EXIT_FAILURE);
   }
   printf("connected to LDAP server %s\n", ldapUri);

   // set verison options
   rc = ldap_set_option(
       ldapHandle,
       LDAP_OPT_PROTOCOL_VERSION, // OPTION
       &ldapVersion);             // IN-Value
   if (rc != LDAP_OPT_SUCCESS)
   {
      fprintf(stderr, "ldap_set_option(PROTOCOL_VERSION): %s\n", ldap_err2string(rc));
      ldap_unbind_ext_s(ldapHandle, NULL, NULL);
      exit(EXIT_FAILURE);
   }

   rc = ldap_start_tls_s(
       ldapHandle,
       NULL,
       NULL);

   if (rc != LDAP_SUCCESS)
   {
      fprintf(stderr, "ldap_start_tls_s(): %s\n", ldap_err2string(rc));
      ldap_unbind_ext_s(ldapHandle, NULL, NULL);
      exit(EXIT_FAILURE);
   }

   BerValue bindCredentials;
   bindCredentials.bv_val = (char *)ldap_pw;
   bindCredentials.bv_len = strlen(ldap_pw);
   BerValue *servercredp; // server's credentials
   rc = ldap_sasl_bind_s(
       ldapHandle,
       ldap_dn,
       LDAP_SASL_SIMPLE,
       &bindCredentials,
       NULL,
       NULL,
       &servercredp);
   if (rc != LDAP_SUCCESS)
   {
      fprintf(stderr, "LDAP bind error: %s\n", ldap_err2string(rc));
      ldap_unbind_ext_s(ldapHandle, NULL, NULL);
      exit(EXIT_FAILURE);
   }

   ////////////////////////////////////////////////////////////////////////////
   // perform ldap search
   // https://linux.die.net/man/3/ldap_search_ext_s
   // _s : synchronous
   // int ldap_search_ext_s(
   //     LDAP *ld,
   //     char *base,
   //     int scope,
   //     char *filter,
   //     char *attrs[],
   //     int attrsonly,
   //     LDAPControl **serverctrls,
   //     LDAPControl **clientctrls,
   //     struct timeval *timeout,
   //     int sizelimit,
   //     LDAPMessage **res );
   LDAPMessage *searchResult;
   rc = ldap_search_ext_s(
       ldapHandle,
       ldapSearchBaseDomainComponent,
       ldapSearchScope,
       ldapSearchFilter,
       (char **)ldapSearchResultAttributes,
       0,
       NULL,
       NULL,
       NULL,
       500,
       &searchResult);
   if (rc != LDAP_SUCCESS)
   {
      fprintf(stderr, "LDAP search error: %s\n", ldap_err2string(rc));
      ldap_unbind_ext_s(ldapHandle, NULL, NULL);
      exit(EXIT_FAILURE);
   }
   // https://linux.die.net/man/3/ldap_count_entries
   printf("Total results: %d\n", ldap_count_entries(ldapHandle, searchResult));

   ////////////////////////////////////////////////////////////////////////////
   // get result of search
   // https://linux.die.net/man/3/ldap_first_entry
   // https://linux.die.net/man/3/ldap_next_entry
   LDAPMessage *searchResultEntry;
   for (searchResultEntry = ldap_first_entry(ldapHandle, searchResult);
        searchResultEntry != NULL;
        searchResultEntry = ldap_next_entry(ldapHandle, searchResultEntry))
   {
      /////////////////////////////////////////////////////////////////////////
      // Base Information of the search result entry
      // https://linux.die.net/man/3/ldap_get_dn
      printf("DN: %s\n", ldap_get_dn(ldapHandle, searchResultEntry));

      /////////////////////////////////////////////////////////////////////////
      // Attributes
      // https://linux.die.net/man/3/ldap_first_attribute
      // https://linux.die.net/man/3/ldap_next_attribute
      //
      // berptr: berptr, a pointer to a BerElement it has allocated to keep
      //         track of its current position. This pointer should be passed
      //         to subsequent calls to ldap_next_attribute() and is used to
      //         effectively step through the entry's attributes.
      BerElement *ber;
      char *searchResultEntryAttribute;
      for (searchResultEntryAttribute = ldap_first_attribute(ldapHandle, searchResultEntry, &ber);
           searchResultEntryAttribute != NULL;
           searchResultEntryAttribute = ldap_next_attribute(ldapHandle, searchResultEntry, ber))
      {
         BerValue **vals;
         if ((vals = ldap_get_values_len(ldapHandle, searchResultEntry, searchResultEntryAttribute)) != NULL)
         {
            for (int i = 0; i < ldap_count_values_len(vals); i++)
            {
               printf("\t%s: %s\n", searchResultEntryAttribute, vals[i]->bv_val);
            }
            ldap_value_free_len(vals);
         }

         // free memory
         ldap_memfree(searchResultEntryAttribute);
      }
      // free memory
      if (ber != NULL)
      {
         ber_free(ber, 0);
      }

      printf("\n");
   }

   // free memory
   ldap_msgfree(searchResult);






    
        

        //LDAP request um zu verifizieren, ob daten passen

        
        //wenn daten passen user in vektor mit authentifizierten usern speichern und ok returnen
        //wenn ok returnt wird nicht vergessen loggedIn bool vom clientController auf true zu setzen

        //ansonten err returnen

        if(true){
            response = "OK\n";
        }
        else{
            response = "ERR\n"; 
        }
    }
    
    // TODO: ADD functions to handle request types
    else if(header == "SEND") {
        // store message
        std::cout << "body: " << body;
        Message mess(body);
        std::cout << mess;
        response = this->StoreMessageToDir(mess,mess.GetReceiver(), "inbox");
        response = this->StoreMessageToDir(mess,mess.GetSender(), "outbox");

        // TODO: test both ^

    } else if (header == "LIST") {
        // retrieve outbox and inbox of user
        std::vector<Message> list = this->GetMessages(body);

        if(list.size() == 0) {
            response = "ERR\n";
            return response;
        }
        // amount of messages
        response = "Results: " + std::to_string(list.size()) + "\n";
        
        int index = 0;
        // all message subjects
        for(auto& mess : list) {
            response.append(std::to_string(index) + ": " + mess.GetSubject() + "\n");
            index++;
        }
    } else if (header == "READ") {
        std::string username = body.substr(0, body.find('\n'));
        int index = std::stoi(body.substr(body.find("\n") + 1, std::string::npos));

        std::vector<Message> list = this->GetMessages(username);

        if(index > list.size() || list.size() == 0){
            response = "ERR\n";
        }
        else{
            response = "OK\n"; 
            response += list.at(index).ToString();
        }

    } else if (header == "DEL") {
        // split body at new line
        std::string username = body.substr(0, body.find('\n'));
        int index = std::stoi(body.substr(body.find("\n") + 1, std::string::npos));

        std::vector<Message> list = this->GetMessages(username);
                
        Message messageToDelete = list.at(index);
        if( this->DeleteMessage(messageToDelete) == -1 ) {
            response = "ERR\n";
        } else {
            response = "OK\n";
        }

    } else {
        std::cout << "[ERROR] Wrong Request Type" << std::endl;
        response = "ERR\n";
    }

    return response;

}

int ServerController::StoreMessageToDir(Message message, std::string user, std::string subfolder)
{
    FILE* file;
    errno = 0;

    std::string filePath = this->baseDir + "/" + user;
    mkdir(filePath.c_str(), 0777);

    filePath += "/" + subfolder;
    mkdir(filePath.c_str(), 0777);

    filePath += "/" + message.GetIdentifier();

    if((file = fopen(filePath.c_str(), "w")) == nullptr) {
        printf("Error: %d (%s)\n", errno, strerror(errno));
        std::cout << "[ERROR] Could not open file \"" << filePath << "\"" << std::endl;
        return -1;
    }

    if(fprintf(file, "%s", message.ToString().c_str()) < 0 ) {
        std::cout << "[ERROR] Could not write to file \"" << filePath << "\"" << std::endl;
        return -1;
    }

    fclose(file);

    return 1;
}

std::vector<Message> ServerController::GetMessages(std::string name)
{
    // TODO: Not working just an idea on how to implement
    name.erase(std::remove(name.begin(), name.end(), '\n'), name.cend());

    std::vector<Message> messagesSent = this->GetMessagesFromDir(name + "/inbox");
    std::vector<Message> messagesReceived = this->GetMessagesFromDir(name + "/outbox");
    std::vector<Message> messagesCombined;

    // Not sure if thats the best way 
    messagesCombined.reserve(messagesReceived.size() + messagesSent.size());
    messagesCombined.insert(messagesCombined.end(), messagesSent.begin(), messagesSent.end());
    messagesCombined.insert(messagesCombined.end(), messagesReceived.begin(), messagesReceived.end());

    if(messagesCombined.size() == 0) {
        std::cout << "[ERROR] No Messages found" << std::endl;
    }

    return messagesCombined;
}

int ServerController::DeleteMessage(Message message)
{
    int result = 0;

    std::string filePathOutbox = this->baseDir + "/" + message.GetSender() + "/outbox/" + message.GetIdentifier();
    result += remove(filePathOutbox.c_str());

    std::string filePathInbox = this->baseDir + "/" + message.GetReceiver() + "/inbox/" + message.GetIdentifier();
    result += remove(filePathInbox.c_str());

    if(result != 0)
        return -1;
    
    return 0;
}

std::vector<Message> ServerController::GetMessagesFromDir(std::string filepath)
{
    // TODO: get it to work? not tested lol
    std::vector<Message> messages;

    struct dirent* dirent_ptr;
    // struct stat statbuf;
    DIR* dir_ptr;
    FILE* file;

    filepath = this->baseDir + "/" + filepath;

    // open dir or return if unsuccessful
    if ((dir_ptr = opendir(filepath.c_str())) == NULL){
        std::cout << "[ERROR] Could not open directory: \"" << filepath << "\"" << std::endl;
        return messages;
    }

    // read dir and loop over all elements inside folder
    while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
        char buffer[2048];
        std::string fileName = dirent_ptr->d_name;

        // these two elements can be skipped
        if(fileName.compare(".") == 0 || fileName.compare("..") == 0)
            continue;
        
        // TODO: open file and store in vector
        std::string absoluteFileName = filepath + "/" + fileName;
        
        if((file = fopen(absoluteFileName.c_str(), "r")) == nullptr) {
            printf("Error: %d (%s)\n", errno, strerror(errno));
            std::cout << "[ERROR] Could not open file \"" << absoluteFileName << "\"" << std::endl;
            continue;
        }

        std::string messageString;
        while( (fgets(buffer , 2048 , file)) != NULL) {
            messageString.append(buffer);
        }

        Message temp(messageString, fileName);
        messages.push_back(temp);

        fclose(file);
    }

    closedir(dir_ptr);

    return messages;
}