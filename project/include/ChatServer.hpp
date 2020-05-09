#ifndef PROJECTKONAMI_CHATSERVER_HPP
#define PROJECTKONAMI_CHATSERVER_HPP

#include <Wt/WServer.h>
#include <mutex>
#include <ctime>
#include <utility>

#include "Auth.hpp"
#include "SessionManager.hpp"
#include "ChatEvent.hpp"
#include "DialogueService.hpp"

// a reference to client
class Client {
public:
    virtual void connect() = 0;
    virtual void disconnect() = 0;

    virtual void sign_out() = 0;

    virtual std::string username() = 0;
};

class ChatServer : public Wt::WServer {
public:
    ChatServer(Wt::WServer& server);

    // delete
    ChatServer(const ChatServer&) = delete;
    ChatServer& operator=(const ChatServer&) = delete;

    // The passed callback method is posted to when a new chat event is received
    // Return false if the client was already connected
    bool connect(Client *client, const ChatEventCallback& handle_event);
    bool disconnect(Client *client);

    std::vector<chat::Dialogue> get_dialogues(const Wt::WString& username);
    std::vector<chat::Message> get_messages(uint dialogue_id);
    void send_msg(const chat::Message& message, const chat::User& user);

    bool create_dialogue(const Wt::WString& creater, const Wt::WString& receiver);

    uint get_user_id(const Wt::WString& username);

    // Try to sign in with given username and password.
    // Returns false if the login was not successful;
    bool sign_in(const Wt::WString& username, const Wt::WString& password);
    bool sign_up(const Wt::WString& username, const Wt::WString& password);

    bool sign_out(const Wt::WString& username);

    std::set<Wt::WString> online_users();
    std::vector<Wt::WString> get_all_users();

    void notify_user(const ChatEvent& event);

private:
    Wt::WServer& server_;
    std::recursive_mutex mutex_;

    SessionManager        sessionManager_;
    Auth                  authService_;
    chat::DialogueService dialogue_service_;
    // chat::UserService     user_service_;

    std::set<Wt::WString> online_users_;

    void post_chat_event(const ChatEvent& event);
};


#endif //PROJECTKONAMI_CHATSERVER_HPP