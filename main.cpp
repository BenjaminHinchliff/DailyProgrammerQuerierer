// std
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <filesystem>
#include <algorithm>
// boost
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
// external
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace po = boost::program_options;

const cpr::Header agent{ {"user-agent", "DailyProgrammerQuerierer by SuniTheFish"} };

// authenticates with reddit, returns token string or throws runtime exception
std::string authenticate()
{
    const auto tokenRequest = cpr::Post(
        cpr::Url{ "https://www.reddit.com/api/v1/access_token" },
        agent,
        cpr::Authentication{ "QWVs7D0qMzHIuA", "" },
        cpr::Payload{
            { "grant_type", "https://oauth.reddit.com/grants/installed_client" },
            { "device_id", "DO_NOT_TRACK_THIS_DEVICE" }
        }
    );
    if (tokenRequest.status_code == 200)
    {
        return json::parse(tokenRequest.text)["access_token"];
    }
    else
    {
        throw new std::runtime_error{ "Token authorization failed with code: " + std::to_string(tokenRequest.status_code) };
    }
}

int getLatestChallengeNumber(cpr::Header oauthHeader)
{
    cpr::Header requestHeaders{ oauthHeader };
    requestHeaders.insert(agent.begin(), agent.end());
    const cpr::Response request{ cpr::Get(
        cpr::Url{ "https://oauth.reddit.com/r/dailyprogrammer/new" },
        requestHeaders,
        cpr::Parameters{
            {"limit", "1"}
        }
    ) };
    const std::string title{ json::parse(request.text)["data"]["children"][0]["data"]["title"] };
    size_t numStart = title.find('#') + 1;
    return std::stoi(title.substr(numStart, title.find(' ', numStart) - numStart));
}

int main(int argc, char* argv[])
{
    // parse options
    po::options_description desc{ "Options" };
    std::string challengeRange;
    std::vector<std::string> difficulties;
    std::string outputPath;
    const std::vector<std::string> defaultDifficulties{ "easy", "intermediate", "hard" };
    desc.add_options()
        ("help,h", "print help message to stdout")
        ("number,n", po::value<std::string>(&challengeRange)->required(), "number of the challenges to download or range or all) (positional)")
        (
            "difficulties,d",
            po::value<std::vector<std::string>>(&difficulties)
                ->multitoken()
                ->default_value(defaultDifficulties, "[" + boost::algorithm::join(defaultDifficulties, ", ") + "]"),
            "the difficulties of the challenges to download (positional after number)"
        )
        ("output,o", po::value<std::string>(&outputPath)->default_value("."), "the output path for the files (optional)")
    ;

    po::positional_options_description positionals;
    positionals.add("number", 1);
    positionals.add("difficulties", -1);

    po::variables_map vm;
    po::store(
        po::command_line_parser(argc, argv)
            .options(desc)
            .positional(positionals)
            .run(),
        vm
    );
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << '\n';
        return 0;
    }

    // authenticate with reddit
    std::string token;
    try
    {
        token = authenticate();
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    const cpr::Header oauthHeader{ {"Authorization", "bearer " + token} };

    // compute min and max challenges
    int minChallenge = 0;
    int maxChallenge = 0;
    if (challengeRange == "all")
    {
        maxChallenge = getLatestChallengeNumber(oauthHeader);
    }
    else if (challengeRange.find('-') != std::string::npos)
    {
        std::vector<std::string> challengeParts{};
        boost::split(challengeParts, challengeRange, boost::is_any_of("-"));
        minChallenge = std::stoi(challengeParts[0]);
        maxChallenge = std::stoi(challengeParts[1]);
    }
    else
    {
        minChallenge = maxChallenge = std::stoi(challengeRange);
    }

    const cpr::Url oauthBase{ "https://oauth.reddit.com/" };
    cpr::Header queryHeaders{ oauthHeader };
    queryHeaders.insert(agent.begin(), agent.end());
    std::vector<cpr::AsyncResponse> requests{};

    for (int challenge = minChallenge; challenge <= maxChallenge; ++challenge)
    {
        for (const std::string& diff : difficulties)
        {
            requests.push_back(
                cpr::GetAsync(
                    oauthBase + "r/dailyprogrammer/search",
                    cpr::Parameters{
                        {"t", "all"},
                        {"sort", "relevance"},
                        {"q", '#' + std::to_string(challenge) + " [" + diff + "]"},
                        {"restrict_sr", "true"},
                        {"limit", "1"}
                    },
                    queryHeaders
                )
            );
        }
    }

    // set output path
    std::filesystem::current_path(outputPath);

    // handle requests
    for (auto& asyncRequest : requests)
    {
        const auto request{ asyncRequest.get() };
        const auto postArrData{ json::parse(request.text)["data"]["children"] };
        if (postArrData.size() > 0)
        {
            const auto postData{ postArrData[0]["data"] };
            std::string title{ postData["title"] };
            title.erase(std::remove(title.begin(), title.end(), '\n'), title.end());
            printf("Saving post \"%s\"\n", title.c_str());
            std::ofstream outputData{ title + ".md" };
            outputData << "## [" << title << "](" << std::string{ postData["url"] } << ")\n";
            const std::string authorName{ postData["author"] };
            outputData << "### [" << authorName << "](" << "https://reddit.com/u/" << authorName << "/)\n\n";
            outputData << std::string{ postData["selftext"] } << '\n';
        }
    }
}