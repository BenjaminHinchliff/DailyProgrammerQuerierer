// std
#include <iostream>
// external
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

/* curl request: curl -X POST -d "grant_type=password&username=SuniTheFish&password=6a5rIJet9Ph%26" --user "Jrg8WySvz1ZnUg:BdSrDzLb-XB2omOeSBD67mXWBZM" https://www.reddit.com/api/v1/access_token */

int main(int argc, char* argv[]) {
    auto r = cpr::Post(
        cpr::Url{ "https://www.reddit.com/api/v1/access_token" },
        cpr::Payload{
            { "grant_type", "password" },
            { "username", "SuniTheFish" },
            { "password", "6a5rIJet9Ph&" }
        },
        cpr::Authentication{ "Jrg8WySvz1ZnUg", "BdSrDzLb-XB2omOeSBD67mXWBZM" },
        cpr::Header{
            {"user-agent", "DailyProgrammerQuerierer by /u/SuniTheFish"}
        }
    );
    if (r.status_code != 200)
    {
        std::cerr << "Token request failed with code: " << r.status_code << '\n';
        std::cerr << "Response: " << r.text << '\n';
        return 1;
    }
}