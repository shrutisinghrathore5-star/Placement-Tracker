#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>

enum class Status {
    Applied,
    Shortlisted,
    Selected,
    Rejected
};

std::string statusToString(Status s);
Status statusFromString(const std::string& s);

class Application {
private:
    int appId = 0;
    int studentRoll = 0;
    int companyId = 0;
    Status status = Status::Applied;
    std::string dateApplied;

public:
    Application() = default;
    Application(int id, int roll, int compId, Status st, std::string date)
        : appId(id), studentRoll(roll), companyId(compId), status(st), dateApplied(std::move(date)) {}

    int getId() const { return appId; }
    int getStudentRoll() const { return studentRoll; }
    int getCompanyId() const { return companyId; }
    Status getStatus() const { return status; }
    const std::string& getDate() const { return dateApplied; }

    void setStatus(Status s) { status = s; }

    // Persistence: "appId|studentRoll|companyId|status|date"
    std::string serialize() const;
    static Application deserialize(const std::string& line);
};

#endif // APPLICATION_HPP
