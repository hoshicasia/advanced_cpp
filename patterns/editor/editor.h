#pragma once
#include <string>
#include <vector>
#include <memory>

class Command {
public:
    virtual void Execute() = 0;
    virtual void Undo() = 0;
    virtual ~Command() = default;
};

class Editor;

class TypeCmd : public Command {
    friend Editor;

public:
    TypeCmd(Editor* editor, char c) {
        editor_ = editor;
        cur_ = c;
    }

    void Execute() override;

    void Undo() override;

private:
    Editor* editor_ = nullptr;
    char cur_ = '\0';
    char prev_ = '\0';
    bool prev_exist_ = false;
};

class ShiftCmd : public Command {
    friend Editor;

public:
    ShiftCmd(Editor* editor, size_t shift) {
        editor_ = editor;
        shift_ = shift;
    }

    void Execute() override;

    void Undo() override;

private:
    Editor* editor_ = nullptr;
    size_t shift_ = 0;
};

class BackSpaceCmd : public Command {
    friend Editor;

public:
    BackSpaceCmd(Editor* editor) {
        editor_ = editor;
    }
    void Execute() override;

    void Undo() override;

private:
    Editor* editor_ = nullptr;
    char del_char_ = '\0';
};

class Editor {
    friend TypeCmd;
    friend ShiftCmd;
    friend BackSpaceCmd;

private:
    std::string text_;
    size_t pos_ = 0;

public:
    void Type(char c) {
        AddCommand(std::make_unique<TypeCmd>(this, c));
    }

    void ShiftLeft() {
        if (pos_ != 0) {
            AddCommand(std::make_unique<ShiftCmd>(this, -1));
        }
    }

    void ShiftRight() {
        if (pos_ < text_.size()) {
            AddCommand(std::make_unique<ShiftCmd>(this, 1));
        }
    }

    void Backspace() {
        if (pos_ != 0) {
            AddCommand(std::make_unique<BackSpaceCmd>(this));
        }
    }

    void Undo() {
        if (!commands_.empty()) {
            commands_.back()->Undo();
            undone_.push_back(std::move(commands_.back()));
            commands_.pop_back();
        }
    }

    void Redo() {
        if (!undone_.empty()) {
            undone_.back()->Execute();
            commands_.push_back(std::move(undone_.back()));
            undone_.pop_back();
        }
    }

    const std::string& GetText() const {
        return text_;
    }

private:
    // void AppendText(const std::string& text);
    // void PopBack(size_t len);

    void AddCommand(std::unique_ptr<Command> cmd) {
        cmd->Execute();
        undone_.clear();
        commands_.push_back(std::move(cmd));
    }

    std::vector<std::unique_ptr<Command>> commands_;
    std::vector<std::unique_ptr<Command>> undone_;
};

void TypeCmd::Execute() {
    if (editor_->pos_ < editor_->text_.size()) {
        prev_ = editor_->text_[editor_->pos_];
        prev_exist_ = true;
    }
    editor_->text_.insert(editor_->pos_, 1, cur_);
    editor_->pos_ += 1;
}

void TypeCmd::Undo() {
    editor_->pos_ -= 1;
    editor_->text_.erase(editor_->pos_, 1);
    if (prev_exist_) {
        editor_->text_.insert(editor_->pos_, 1, prev_);
    }
}

void ShiftCmd::Execute() {
    editor_->pos_ += shift_;
}

void ShiftCmd::Undo() {
    editor_->pos_ -= shift_;
}

void BackSpaceCmd::Execute() {
    editor_->pos_ -= 1;
    del_char_ = editor_->text_[editor_->pos_];
    editor_->text_.erase(editor_->pos_, 1);
}

void BackSpaceCmd::Undo() {
    editor_->text_.insert(editor_->pos_, 1, del_char_);
    editor_->pos_ += 1;
}