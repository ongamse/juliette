//
// Created by Alan Tao on 9/2/2022.
//

#include <chrono>
#include <unordered_map>

#include "uci.h"
#include "util.h"
#include "tables.h"
#include "search.h"
#include "bitboard.h"

#define BUFLEN 512

std::map<std::string, std::string> options;

const char *id_str = "id name juliette author Alan Tao";
std::string replies[] = {"id", "uciok", "readyok", "bestmove", "copyprotection", "registration", "info_t", "option"};

#define id 0
#define uciok 1
#define readyok 2
#define bestmove 3
#define copyprotection 4
#define registration 5
#define uci_info 6
#define option 7

extern bitboard board;
extern stack_t *stack;
bool board_initialized = false;

/* Engine should use clientSocket to send reply to GUI */
SOCKET clientSocket;
char sendbuf[BUFLEN];

extern int source;

// DELETE ME:
extern std::unordered_map<uint64_t, RTEntry> repetition_table;

void initialize_UCI(SOCKET cs) {
    clientSocket = cs;
    options.insert(std::pair<std::string, std::string>("OwnBook", "off"));
    options.insert(std::pair<std::string, std::string>("debug", "off"));
}

void parse_UCI_string(const char *uci) {
    std::string uci_string(uci), buff;
    size_t i = 0;

    while (i < uci_string.length() && uci_string.at(i) != ' ') {
        buff.push_back(uci_string.at(i));
        ++i;
    }
    /* Skips all spaces */
    while (i < uci_string.length() && uci_string.at(i) == ' ') {
        ++i;
    }
    std::string args;
    while (i < uci_string.length()) {
        args.push_back(uci_string.at(i++));
    }
    if (buff == "uci") {
        size_t len = strlen(id_str);
        memcpy(sendbuf, id_str, len); // NOLINT(bugprone-not-null-terminated-result)
        sendbuf[len] = '\n';
        strcpy(&sendbuf[len + 1], replies[uciok].c_str());
        reply();
    } else if (buff == "ucinewgame") {
        board_initialized = false;
        initialize_zobrist();
    } else if (buff == "position") {
        position(args);
    } else if (buff == "go") {
        go(args);
    } else if (buff == "quit") {
        std::cout << "juliette:: bye! i enjoyed playing with you :)" << std::endl;
        exit(0);
    }
}

std::vector<std::string> split(std::string &input) {
    std::vector<std::string> args;
    std::string tok;
    size_t p;

    while ((p = input.find(' ')) != std::string::npos) {
        tok = input.substr(0, p);
        args.push_back(tok);
        input.erase(0, p + 1);
    }
    return args;
}

void position(std::string &arg) {
    if (arg == "startpos") {
        init_board(START_POSITION);
    } else {
        init_board(arg.c_str());
    }
    board_initialized = true;
}

void go(std::string &args) {
    std::vector<std::string> argv = split(args);
    // TODO: Configure function based on provided arguments according to UCI protocol.
    if (!board_initialized) {
        // TODO: Error handling for uninitialized board
    }
    auto start = std::chrono::steady_clock::now();
    info_t result = search((int16_t) 6);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << '\n';
    sprintf(sendbuf, "%s %c%d%c%d", replies[bestmove].c_str(),
            file_of(result.best_move.from) + 'a', rank_of(result.best_move.from) + 1,
            file_of(result.best_move.to) + 'a', rank_of(result.best_move.to) + 1);
    reply();
}

void reply() {
    if (source == 0) {
        send(clientSocket, sendbuf, (int) strlen(sendbuf), 0);
    } else if (source == 1) {
        std::cout << sendbuf << std::endl;
    }
}