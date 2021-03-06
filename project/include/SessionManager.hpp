#ifndef MESSENGER_PROJECT_SESSIONMANAGER_HPP
#define MESSENGER_PROJECT_SESSIONMANAGER_HPP

#include <vector>
#include <stdint.h>
#include <memory>

#include <Wt/WEvent.h>
#include <Wt/WSignal.h>

#include "Scheduler.hpp"
#include "Session.hpp"

class Client;

class SessionManager {
private:
    using ClientMap = std::map<Client *, Session>;
    ClientMap active_sessions_;
//
//    std::vector<Session> inactive_sessions_;
//    std::vector<Session> recently_closed_sessions_;

    std::map<std::string, uint32_t> reserved_map_;

    Scheduler scheduler_;

public:
    SessionManager();
    SessionManager(const SessionManager&) = delete;
    ~SessionManager() = default;

    bool add_session(Client *client, const Session& s); // вызывается модулем Auth при успешной авторизации (Session создается в Auth)
    bool close_session(Client *client); // вызывается при принудительном завершении сессии

    void set_cookie(const std::string& username, const std::string& cookie);

    const ClientMap& active_sessions() const { return active_sessions_; }
    uint32_t user_id(const std::string& username) { return reserved_map_[username]; }

    void reserve(const std::string& username, uint32_t user_id);
    void unreserve(const std::string& username);
    bool has_reserved(const std::string& username);

    bool add_session(std::unique_ptr<Session> s) { return false;};
    std::vector<uint32_t> get_online_users(); // вызывается ядром
    uint32_t get_id_by_token(std::string& token);

private:
    void check_inactive_sessions(); // пробегается по списку автивных сессий, если (time::now - s.time_point) > 5 min ---> s.deactivate
    void delete_inactive_sessions();  //
};


#endif //MESSENGER_PROJECT_SESSIONMANAGER_HPP
