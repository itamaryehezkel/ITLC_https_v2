#ifndef HELPERS_H
#define HELPERS_H

int find(const char *str, char delim) {
    char *pos = memchr(str, delim, strlen(str));
    return (pos) ? (pos - str) : -1;
}

unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}



int count_crlf(const char *str) {
    int count = 0;
    while (*str && *(str + 1)) {
        if (*str == '\r' && *(str + 1) == '\n') {
            count++;
            str += 2;
        } else {
            str++;
        }
    }
    return count;
}

int search(int arr[], int size, int target) {
    int left = 0, right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target)
            return mid;
        else if (arr[mid] < target)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}



const char *get_mime_type(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot) return "application/octet-stream";
    const char *ext = dot + 1;

    for (int i = 0; mime_map[i].extension != NULL; ++i) {
        if (strcasecmp(ext, mime_map[i].extension) == 0) {
            return mime_map[i].mime_type;
        }
    }
    return "application/octet-stream";
}



void print_signal_error(int sig_num) {
    const char *sig_name;

    switch (sig_num) {
        case SIGHUP:    sig_name = "SIGHUP (Hangup detected)"; break;
        case SIGINT:    sig_name = "SIGINT (Interrupt from keyboard)"; break;
        case SIGQUIT:   sig_name = "SIGQUIT (Quit from keyboard)"; break;
        case SIGILL:    sig_name = "SIGILL (Illegal instruction)"; break;
        case SIGTRAP:   sig_name = "SIGTRAP (Trace/breakpoint trap)"; break;
        case SIGABRT:   sig_name = "SIGABRT (Abort signal)"; break;
        case SIGBUS:    sig_name = "SIGBUS (Bus error)"; break;
        case SIGFPE:    sig_name = "SIGFPE (Floating point exception)"; break;
        case SIGKILL:   sig_name = "SIGKILL (Kill signal)"; break;
        case SIGUSR1:   sig_name = "SIGUSR1 (User-defined signal 1)"; break;
        case SIGSEGV:   sig_name = "SIGSEGV (Segmentation fault)"; break;
        case SIGUSR2:   sig_name = "SIGUSR2 (User-defined signal 2)"; break;
        case SIGPIPE:   sig_name = "SIGPIPE (Broken pipe)"; break;
        case SIGALRM:   sig_name = "SIGALRM (Timer signal)"; break;
        case SIGTERM:   sig_name = "SIGTERM (Termination signal)"; break;
        case SIGSTKFLT: sig_name = "SIGSTKFLT (Stack fault on coprocessor)"; break;
        case SIGCHLD:   sig_name = "SIGCHLD (Child stopped or terminated)"; break;
        case SIGCONT:   sig_name = "SIGCONT (Continue if stopped)"; break;
        case SIGSTOP:   sig_name = "SIGSTOP (Stop process)"; break;
        case SIGTSTP:   sig_name = "SIGTSTP (Stop typed at terminal)"; break;
        case SIGTTIN:   sig_name = "SIGTTIN (Terminal input for background process)"; break;
        case SIGTTOU:   sig_name = "SIGTTOU (Terminal output for background process)"; break;
        case SIGURG:    sig_name = "SIGURG (Urgent condition on socket)"; break;
        case SIGXCPU:   sig_name = "SIGXCPU (CPU time limit exceeded)"; break;
        case SIGXFSZ:   sig_name = "SIGXFSZ (File size limit exceeded)"; break;
        case SIGVTALRM: sig_name = "SIGVTALRM (Virtual alarm clock)"; break;
        case SIGPROF:   sig_name = "SIGPROF (Profiling timer expired)"; break;
        case SIGWINCH:  sig_name = "SIGWINCH (Window size change)"; break;
        case SIGIO:     sig_name = "SIGIO (I/O now possible)"; break;
        case SIGPWR:    sig_name = "SIGPWR (Power failure)"; break;
        case SIGSYS:    sig_name = "SIGSYS (Bad system call)"; break;
        default:        sig_name = "Unknown signal"; break;
    }

    fprintf(stdout, "Received signal %d: %s\n", sig_num, sig_name);
}
void handler(int sig) {
    print_signal_error(sig);
//    fprintf(stdout, "Caught signal %d\n", sig);
//    exit(EXIT_FAILURE);
}

void bind_signal_handlers(){
    signal(SIGHUP, handler);
//    signal(SIGINT, handler);
//    signal(SIGQUIT, handler);
    signal(SIGILL, handler);
    signal(SIGTRAP, handler);
    signal(SIGABRT, handler);
#ifdef SIGIOT
    signal(SIGIOT, handler);
#endif
    signal(SIGBUS, handler);
    signal(SIGFPE, handler);
//    signal(SIGKILL, handler); // Cannot be caught or ignored
    signal(SIGUSR1, handler);
    signal(SIGSEGV, handler);
    signal(SIGUSR2, handler);
    signal(SIGPIPE, handler);
    signal(SIGALRM, handler);
//    signal(SIGTERM, handler);
    signal(SIGCHLD, handler);
    signal(SIGCONT, handler);
  //  signal(SIGSTOP, handler); // Cannot be caught or ignored
    signal(SIGTSTP, handler);
    signal(SIGTTIN, handler);
    signal(SIGTTOU, handler);
    signal(SIGURG, handler);
    signal(SIGXCPU, handler);
    signal(SIGXFSZ, handler);
    signal(SIGVTALRM, handler);
    signal(SIGPROF, handler);
    signal(SIGWINCH, handler);
    signal(SIGPOLL, handler);
#ifdef SIGPWR
    signal(SIGPWR, handler);
#endif
#ifdef SIGSYS
    signal(SIGSYS, handler);
#endif
}


#endif
