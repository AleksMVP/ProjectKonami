#ifndef PROJECTKONAMI_CHATWIDGET_HPP
#define PROJECTKONAMI_CHATWIDGET_HPP

#include <Wt/WContainerWidget.h>
#include <Wt/WMessageBox.h>

#include "ChatServer.hpp"

class ChatWidget : public Wt::WContainerWidget, public Client {
public:
    ChatWidget(const Wt::WString& username, ChatServer& server);
    ~ChatWidget();

    void connect() override;
    void disconnect() override;

    void sign_out() override;

    std::string username() override { return username_.toUTF8(); }

private:
    ChatServer& server_;
    Wt::WString username_;


//    Session session_;
    uint32_t  user_id_;

    Wt::JSlot                    clearInput_;

    Wt::WContainerWidget         *messages_;
    Wt::WTextArea                *messageEdit_;
    Wt::Core::observing_ptr<Wt::WPushButton> sendButton_;

    bool is_uploaded;

    Wt::Core::observing_ptr<Wt::WContainerWidget> userList_;
    Wt::Core::observing_ptr<Wt::WContainerWidget> dialoguesList_;
    Wt::Core::observing_ptr<Wt::WContainerWidget> fileUploader_;

    Wt::WFileUpload* fileUploaderPtr_;


    std::map<Wt::WString, chat::Dialogue> dialogues_;
    Wt::WString current_dialogue_;

    void fill_fileuploader();
    void create_UI();
    void create_layout(std::unique_ptr<Wt::WWidget> messages, 
                    std::unique_ptr<Wt::WWidget> userList,
                    std::unique_ptr<Wt::WWidget> messageEdit,
                    std::unique_ptr<Wt::WWidget> sendButton, 
                    std::unique_ptr<Wt::WWidget> logoutButton,
                    std::unique_ptr<Wt::WWidget> chatUserList,
                    std::unique_ptr<Wt::WWidget> fileUploader);

    void process_chat_event(const ChatEvent& event);

    chat::Dialogue get_dialogue(const Wt::WString& dialogue_name) {return dialogues_[dialogue_name];}
    std::string get_message_format(const std::string& username, 
                                   const std::string& message_content, 
                                   const time_t& time);

    void update_users_list();
    void update_dialogue_list();
    void update_messages(const Wt::WString& username);
    bool create_dialogue(const Wt::WString& username);
    void send_message();

    std::string copy_file(const std::string& file_path, const std::string& filename);

};


#endif //PROJECTKONAMI_CHATWIDGET_HPP