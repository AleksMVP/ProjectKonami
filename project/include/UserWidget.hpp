#ifndef PROJECTKONAMI_USERWIDGET_HPP
#define PROJECTKONAMI_USERWIDGET_HPP

#include <Wt/WContainerWidget.h>
#include <Wt/WImage.h>
#include <Wt/WLink.h>
#include <Wt/WText.h>
#include <Wt/WGroupBox.h>
#include_next <Wt/WHBoxLayout.h>

const std::string default_avatar = "./avatars/88/img_template_4.jpg";

class UserWidget : public Wt::WContainerWidget {
public:
    UserWidget(const Wt::WString& username, const std::string& path)
            : username_(username),
            avatar_link_(path) {
        auto Layout = std::make_unique<Wt::WHBoxLayout>();

        if (!path.empty())
            avatar_ = Layout->addWidget(std::make_unique<Wt::WImage>(Wt::WLink(avatar_link_)));
        else
            avatar_ = Layout->addWidget(std::make_unique<Wt::WImage>(Wt::WLink(default_avatar)));

    //    avatar_->setMargin(30, Wt::WFlags<Wt::Side>::enum_type::Right);
        auto *displayName = Layout->addWidget(std::make_unique<Wt::WText>(username), 1);
        Layout->setContentsMargins(0, 0 ,0 ,0);
        setLayout(std::move(Layout));
        setStyleClass("user-widget");
    };

    Wt::WString get_username() {return username_;};

private:
    Wt::WString username_;
    Wt::WLink avatar_link_;
    Wt::WImage* avatar_;
};


#endif //PROJECTKONAMI_USERWIDGET_HPP
