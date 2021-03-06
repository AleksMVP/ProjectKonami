#include <Wt/WLineEdit.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WText.h>
#include <Wt/WLabel.h>
#include <Wt/WProgressBar.h>
#include <fstream>

#include "RegistrationForm.hpp"
#define MINPASSLENGTH 4

RegistrationForm::RegistrationForm()
    :Wt::WContainerWidget(),
    is_valid_(true),
    error_(ErrorType::None),
    validator_()
{
    setFloatSide(Wt::Side::CenterX);
    create_UI();
}

void RegistrationForm::create_UI() {
    auto vLayout = setLayout(std::make_unique<Wt::WVBoxLayout>());
    vLayout->setContentsMargins(10,10,10,10);

    auto line = vLayout->addWidget(std::make_unique<Wt::WContainerWidget>(), 0,  Wt::AlignmentFlag::Middle);
    auto label_ = std::make_unique<Wt::WLabel>("User name: ");
    auto label = label_.get();
    line->addWidget(std::move(label_));
    username_edit_ = line->addWidget(std::make_unique<Wt::WLineEdit>());
    username_edit_->setFloatSide(Wt::Side::Right);
    username_edit_->setMinimumSize(150, 16);
    username_edit_->setMaximumSize(150, 16);
    label->setBuddy(username_edit_);

    line = vLayout->addWidget(std::make_unique<Wt::WContainerWidget>(), 0,  Wt::AlignmentFlag::Middle);
    line->addWidget(std::make_unique<Wt::WText>("Choose password: "));
    password_edit_first_ = line->addWidget(std::make_unique<Wt::WLineEdit>());
    password_edit_first_->setFloatSide(Wt::Side::Right);
    password_edit_first_->setMinimumSize(150, 16);
    password_edit_first_->setMaximumSize(150, 16);
    password_edit_first_->setAttributeValue("type", "password");

    line = vLayout->addWidget(std::make_unique<Wt::WContainerWidget>(), 0,  Wt::AlignmentFlag::Middle);
    line->addWidget(std::make_unique<Wt::WText>("Repeat password: "));
    password_edit_second_ = line->addWidget(std::make_unique<Wt::WLineEdit>());
    password_edit_second_->setFloatSide(Wt::Side::Right);
    password_edit_second_->setMinimumSize(150, 16);
    password_edit_second_->setMaximumSize(150, 16);
    password_edit_second_->setAttributeValue("type", "password");

    line = vLayout->addWidget(std::make_unique<Wt::WContainerWidget>(),0,Wt::AlignmentFlag::Middle);
    line->addWidget(std::make_unique<Wt::WText>("Upload avatar: "));
    file_upload_container_ = line->addWidget(std::make_unique<Wt::WContainerWidget>());
    create_file_uploader();

    line = vLayout->addWidget(std::make_unique<Wt::WContainerWidget>(),0,Wt::AlignmentFlag::Middle);
    auto helpText = line->addWidget(std::make_unique<Wt::WText>("Password must be at least 4 characters long. "
                                                "The password must consist of letters of the Latin alphabet (A-z),"
                                                "Arabic numerals (0-9) and special characters."));
    helpText->setStyleClass("help-text");

    line = vLayout->addWidget(std::make_unique<Wt::WContainerWidget>(), 0,  Wt::AlignmentFlag::Middle);
    status_msg_ = line->addWidget(std::make_unique<Wt::WText>());
}

bool RegistrationForm::validate() {
    auto username = username_edit_->text();
    auto password1 = password_edit_first_->text();
    auto password2 = password_edit_second_->text();

    if (password1 != password2) {
        error_ = ErrorType::PasswordMismatch;
        is_valid_ = false;
        return false;
    }

    if (password1.toUTF8().length() < MINPASSLENGTH) {
        error_ = ErrorType::ShortPassword;
        is_valid_ = false;
        return false;
    }

    if (error_ == ErrorType::ImgTooLarge) {
        is_valid_ = false;
        return false;
    }

    auto res = validator_.evaluate_strength(username, password1);

    if (res.isValid()) {
        is_valid_ = true;
        return true;
    } else {
        is_valid_ = false;
        status_string_ = res.message();
        error_ = ErrorType::WeakPassword;
        return false;
    }
}

void RegistrationForm::set_user_exists_error() {
    error_ = ErrorType::UsernameExists;
    is_valid_ = false;
}

void RegistrationForm::create_file_uploader() {
    file_upload_container_->clear();
    file_upload_ = file_upload_container_->addWidget(std::make_unique<Wt::WFileUpload>());
    file_upload_->setInline(true);
    file_upload_->setFileTextSize(100);
    file_upload_->setProgressBar(std::make_unique<Wt::WProgressBar>());
    file_upload_->setFilters("image/*");

    file_upload_->changed().connect([this] {
        file_upload_->upload();
        status_msg_->setText("File upload is changed.");
    });

    file_upload_->uploaded().connect([this] {
        error_ = ErrorType::None;
        status_msg_->setText("File upload is finished.");
    });

    file_upload_->fileTooLarge().connect([=] {
        error_ = ErrorType::ImgTooLarge;
        status_msg_->setText("File is too large (over 4 MB). Try again");
        create_file_uploader();
    });
}
