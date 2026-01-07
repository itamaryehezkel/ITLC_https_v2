#ifndef IO_H
#define IO_H


#define _OPTIONS -2052162639
#define _DELETE -1417066568
#define _CONNECT -415991569
#define _GET 193456677
#define _POST 2089437419
#define _PUT 193467006
#define _PATCH 231455573
#define _TRACE 236789492
#define _HEAD 2089138423

#define HEADER_BUFFER_SIZE 512

#include <time.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <ctype.h>
//#include <mysql/mysql.h>

// #include <openssl/ssl.h>
// #include <openssl/err.h>

#include "helpers.h" //safe
#include "file.h"  //safe

char * get_sys_file(const char* path);
/*
typedef struct{
    my_ulonglong affected_rows;
    unsigned long num_rows;
    unsigned int num_cols;
    char ** fields;
    char *** data;
} SQL_RESULT;

void get_rows(MYSQL *conn, char * query, SQL_RESULT * result);
void get_rows_sql_file(MYSQL *conn, const char* path, SQL_RESULT * result);
void print_sql_result(SQL_RESULT * result);
void free_sql_result(SQL_RESULT * result);
SQL_RESULT select_rows(MYSQL *conn, char * schema, char * table, char * target_fields, char *fields[], char *values[], unsigned int cols, char * logic);
SQL_RESULT insert_row(MYSQL *conn, char * schema, char * table, char *fields[], char *values[], unsigned int cols);
SQL_RESULT update_row(MYSQL *conn, char * schema, char * table, char *fields[], char *values[], unsigned int cols, char *where_name, char *where_value);

// 

void print_sql_result(SQL_RESULT * result){
    fprintf(stdout, "Fetched %d rows.\n", result->num_rows);
    for(unsigned int i = 0; i < result->num_cols; i++)
        fprintf(stdout, "%s\t%s", result->fields[i], strlen(result->fields[i]) < 8 ? "\t" : "");
    fprintf(stdout, "\n");
    for(unsigned int i = 0; i < result->num_rows; i++){
        for(unsigned int j = 0; j < result->num_cols; j++)
            fprintf(stdout, "%s\t%s", result->data[i][j], strlen(result->data[i][j]) < 8 ? "\t" : "");
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}


void free_sql_result(SQL_RESULT * result){
    for(unsigned int i = 0; i < result->num_cols; i++)
        free(result->fields[i]);
    free(result->fields);
    for(unsigned int i = 0; i < result->num_rows; i++){
        for(unsigned int j = 0; j < result->num_cols; j++)
            free(result->data[i][j]);
        free(result->data[i]);
    }
    free(result->data);
}

void get_rows_sql_file(MYSQL *conn, const char* path, SQL_RESULT * result){
    get_rows(conn, get_sys_file(path), result);
}


void get_rows(MYSQL *conn, char * query, SQL_RESULT * result){
    MYSQL_RES *res;
    MYSQL_ROW row;

    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
    } else {
        res = mysql_store_result(conn);
        if (res) {
            result->num_rows = mysql_num_rows(res);
            result->num_cols = mysql_num_fields(res);
            result->fields = (char **)calloc(result->num_cols, sizeof(char *));
            result->data = (char ***)calloc(result->num_rows, sizeof(char **));
            
            MYSQL_FIELD *fields = mysql_fetch_fields(res);

            for (unsigned int i = 0; i < result->num_cols; i++) {
                result->fields[i] = calloc(strlen(fields[i].name)+1, sizeof(char));
                strcpy(result->fields[i], fields[i].name);
                DUMP_SQL_GETTER fprintf(stdout, "%s\t%s", result->fields[i], strlen(result->fields[i]) < 8 ? "\t" : "");
            }
            DUMP_SQL_GETTER fprintf(stdout, "\n");
            
            unsigned int row_num = 0;            
            while ((row = mysql_fetch_row(res))) {

                result->data[row_num] = (char **)calloc(result->num_cols, sizeof(char *));

                for (unsigned int i = 0; i < result->num_cols; i++){
                    char * val = row[i] ? row[i] : "NULL";

                    result->data[row_num][i] = (char *)calloc(strlen(result->fields[i]) + 1, sizeof(char));
                    strcpy(result->data[row_num][i], val);

                    DUMP_SQL_GETTER fprintf(stdout, "%s\t%s", result->data[row_num][i], strlen(result->data[row_num][i]) < 8 ? "\t" : "");
                    // fprintf(stdout, "Column %u: %s\n", i, row[i] ? row[i] : "NULL");
                }
                DUMP_SQL_GETTER fprintf(stdout, "\n");
                row_num++;
            }
            result->affected_rows = mysql_affected_rows(conn);
            mysql_free_result(res);
        }
    }

}

SQL_RESULT select_rows(MYSQL *conn, char * schema, char * table, char * target_fields, char *fields[], char *values[], unsigned int cols, char * logic){
    SQL_RESULT result_seq;
    // char query[1024];
    // sprintf(query, "SELECT %s FROM `%s`.`%s` WHERE `%s` = '%s'", fields, schema, table, "username", "test");
    const char * template = "SELECT %s FROM `%s`.`%s` WHERE ;";
    
    size_t query_length = strlen(template) + strlen(schema) + strlen(table) + strlen(target_fields) + 1;

    
    for(unsigned int i = 0; i < cols; i++)
        query_length += strlen(fields[i]) + strlen(values[i]) + strlen(logic) + 18;
        
    // for(unsigned int i = 0; i < where_count; i++)
    // query_length += strlen(where_name) + 64;

    char * query = calloc(query_length, sizeof(char));

    strcpy(query, "SELECT ");
    
    strcat(query, target_fields);

    strcat(query, " FROM `");
    strcat(query, schema);
    strcat(query, "`.`");
    strcat(query, table);
    strcat(query, "`");
    
    if(cols)
        strcat(query, " WHERE ");
    for(unsigned int i = 0; i < cols; i++){

        strcat(query, "`");
        strcat(query, fields[i]);
        strcat(query, "` = '");
        strcat(query, values[i]);
        strcat(query, "'");
        if(i != cols - 1){
            strcat(query, " ");
            strcat(query, logic);
            strcat(query, " ");
        }
    }
    
    strcat(query, ";");
    get_rows(conn, query, &result_seq);
    return result_seq;
}

SQL_RESULT update_row(MYSQL *conn, char * schema, char * table, char *fields[], char *values[], unsigned int cols, char *where_name, char *where_value){
    
    // UPDATE
    // if (mysql_query(conn, "UPDATE users SET age = 31 WHERE name = 'Alice'")) {
    //     fprintf(stderr, "UPDATE failed: %s\n", mysql_error(conn));
    // }UPDATE `web`.`users` SET `username` = 'asd', `password` = 'qwasdasde' WHERE (`seq` = '0000000011');


    const char * template = "UPDATE ``.`` SET;";
    
    size_t query_length = strlen(template) + strlen(schema) + strlen(table) + 1;

    for(unsigned int i = 0; i < cols; i++)
        query_length += strlen(fields[i]) + strlen(values[i]) + 18;
        
    // for(unsigned int i = 0; i < where_count; i++)
    query_length += strlen(where_name) + 64;

    char * query = calloc(query_length, sizeof(char));

    strcpy(query, "UPDATE `");
    strcat(query, schema);
    strcat(query, "`.`");
    strcat(query, table);
    strcat(query, "` SET ");
    for(unsigned int i = 0; i < cols; i++){
        strcat(query, "`");
        strcat(query, fields[i]);
        strcat(query, "` = '");
        strcat(query, values[i]);
        strcat(query, "'");
        if(i != cols - 1)
            strcat(query, ", ");
    }
    strcat(query, " WHERE (");
    // for(unsigned int i = 0; i < where_count; i++){
        strcat(query, "`");
        strcat(query, where_name);
        strcat(query, "` = '");
        strcat(query, where_value);
        strcat(query, "'");
    //     if(i != cols - 1)
    //         strcat(query, ", ");
    // }
    strcat(query, ");");

    if (mysql_query(conn, query)) {
        fprintf(stderr, "UPDATE failed: %s\n", mysql_error(conn));
    }
    // my_ulonglong affected_rows = mysql_affected_rows(conn);
    return (SQL_RESULT){.affected_rows=mysql_affected_rows(conn)};
}

SQL_RESULT insert_row(MYSQL *conn, char * schema, char * table, char *fields[], char *values[], unsigned int cols){
        // INSERT
    const char * template = "INSERT INTO ``.`` () VALUES ();";
    
    size_t query_length = strlen(template) + strlen(schema) + strlen(table) + 1;
    for(unsigned int i = 0; i < cols; i++)
        query_length += strlen(fields[i]) + strlen(values[i]) + 8;

    char * query = calloc(query_length, sizeof(char));  //"INSERT INTO `web`.`users` (`username`, `password`, `full_name`) VALUES ('asd', 'asd', 'asd');"

    strcpy(query, "INSERT INTO `");
    strcat(query, schema);
    strcat(query, "`.`");
    strcat(query, table);
    strcat(query, "` (");
    for(unsigned int i = 0; i < cols; i++){
        strcat(query, "`");
        strcat(query, fields[i]);
        strcat(query, "`");
        if(i != cols - 1)
            strcat(query, ", ");
    }
    strcat(query, ") VALUES (");
    for(unsigned int i = 0; i < cols; i++){
        strcat(query, "'");
        strcat(query, values[i]);
        strcat(query, "'");
        if(i != cols - 1)
            strcat(query, ", ");
    }
    strcat(query, ");");

    if (mysql_query(conn, query)) {
        fprintf(stderr, "INSERT failed: %s\n", mysql_error(conn));
    }
    // my_ulonglong affected_rows = mysql_affected_rows(conn);
    return (SQL_RESULT){.affected_rows=mysql_affected_rows(conn)};
}
*/



char * get_sys_file(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("fseek failed");
        fclose(file);
        return NULL;
    }

    long size = ftell(file);
    if (size < 0) {
        perror("ftell failed");
        fclose(file);
        return NULL;
    }
    rewind(file);

    unsigned char* buffer = (unsigned char*)calloc(size/sizeof(unsigned char), sizeof(unsigned char));
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    size_t read = fread(buffer, 1, size, file);
    fclose(file);

    if (read != (size_t)size) {
        fprintf(stderr, "Only read %zu of %ld bytes\n", read, size);
        free(buffer);
        return NULL;
    }
    TRACE_SYS_FILE_READ_CALLS fprintf(stdout, "%s\n", buffer);
    return buffer;
}



char * getStatus(int code) {
    switch(code) {
        
        case 100: return "100 Continue";
        case 101: return "101 Switching Protocols";
        case 102: return "102 Processing";
        case 103: return "103 Early Hints";

        
        case 200: return "200 OK";
        case 201: return "201 Created";
        case 202: return "202 Accepted";
        case 203: return "203 Non-Authoritative Information";
        case 204: return "204 No Content";
        case 205: return "205 Reset Content";
        case 206: return "206 Partial Content";
        case 207: return "207 Multi-Status";
        case 208: return "208 Already Reported";
        case 226: return "226 IM Used";

        
        case 300: return "300 Multiple Choices";
        case 301: return "301 Moved Permanently";
        case 302: return "302 Found";
        case 303: return "303 See Other";
        case 304: return "304 Not Modified";
        case 305: return "305 Use Proxy";
        case 306: return "306 (Unused)";
        case 307: return "307 Temporary Redirect";
        case 308: return "308 Permanent Redirect";

        
        case 400: return "400 Bad Request";
        case 401: return "401 Unauthorized";
        case 402: return "402 Payment Required";
        case 403: return "403 Forbidden";
        case 404: return "404 Not Found";
        case 405: return "405 Method Not Allowed";
        case 406: return "406 Not Acceptable";
        case 407: return "407 Proxy Authentication Required";
        case 408: return "408 Request Timeout";
        case 409: return "409 Conflict";
        case 410: return "410 Gone";
        case 411: return "411 Length Required";
        case 412: return "412 Precondition Failed";
        case 413: return "413 Payload Too Large";
        case 414: return "414 URI Too Long";
        case 415: return "415 Unsupported Media Type";
        case 416: return "416 Range Not Satisfiable";
        case 417: return "417 Expectation Failed";
        case 418: return "418 I'm a teapot";
        case 421: return "421 Misdirected Request";
        case 422: return "422 Unprocessable Entity";
        case 423: return "423 Locked";
        case 424: return "424 Failed Dependency";
        case 425: return "425 Too Early";
        case 426: return "426 Upgrade Required";
        case 428: return "428 Precondition Required";
        case 429: return "429 Too Many Requests";
        case 431: return "431 Request Header Fields Too Large";
        case 451: return "451 Unavailable For Legal Reasons";

        case 500: return "500 Internal Server Error";
        case 501: return "501 Not Implemented";
        case 502: return "502 Bad Gateway";
        case 503: return "503 Service Unavailable";
        case 504: return "504 Gateway Timeout";
        case 505: return "505 HTTP Version Not Supported";
        case 506: return "506 Variant Also Negotiates";
        case 507: return "507 Insufficient Storage";
        case 508: return "508 Loop Detected";
        case 510: return "510 Not Extended";
        case 511: return "511 Network Authentication Required";

        // Default fallback
        default: return "500 Internal Server Error";
    }
}



void *handle_client(void *arg);


// typedef enum {
//     GET,
//     POST,
//     DELETE,
//     PUT,
//     PATCH,
//     OPTIONS,
//     CONNECT,
//     TRACE,
//     HEAD,
//     UNSUPPORTED_METHOD
// } Method;

#define READ_MAX_RETRIES 200
#define READ_TIMEOUT_SECS 5

#define BUFFER_SIZE 8192
#define MAX_URI_LENGTH 4096
#define MAX_HEADER_BLOCK 65536

typedef enum {
    M_OPTIONS,
    M_DELETE,
    M_CONNECT,
    M_GET,
    M_PUT,
    M_PATCH,
    M_TRACE,
    M_HEAD,
    M_POST,
    M_UNSUPPORTED
} Method;
typedef struct {
    int sock;
    SSL_CTX *ctx;
    SSL *ssl;
    struct sockaddr_in address;
} connection_t;


char * method_to_str(Method method){
  switch(method){
    case M_OPTIONS: return "OPTIONS";
    case M_DELETE:  return "DELETE";
    case M_CONNECT: return "CONNECT";
    case M_GET:     return "GET";
    case M_PUT:     return "PUT";
    case M_PATCH:   return "PATCH";
    case M_TRACE:   return "TRACE";
    case M_HEAD:    return "HEAD";
    case M_POST:    return "POST";
    default:      return "UNSUPPORTED_METHOD";
  }
}

 //Request holds offsets and lengths (all relative to buffer) */

typedef struct {
    connection_t *con;
    Method method;
    char ip[INET_ADDRSTRLEN];

    /* buffer holding request-line + headers + body (no copies) */
    char *buffer;
    int buffer_size;
    int total_read;

    /* request-line parts */
    int reqline_start; /* always 0 */
    int reqline_len;

    /* version */
    int version_start;
    int version_len;

    /* URI */
    int uri_start;
    int uri_len;
    int uri_query_len; /* 0 if no query */

    /* headers block */
    int headers_start;
    int headers_len; /* includes final CRLFCRLF */

    /* body */
    int body_start;
    int body_len;
    char * host;

} Request;


/* Prototypes */
const char *req_method_str(const Request *req);
const char *req_method_short(const Request *req); /* alternative short name */
const char *req_uri_ptr(const Request *req, int *out_len); /* path only (no query) */
const char *req_query_ptr(const Request *req, int *out_len); /* query part (after ?) */
const char *req_version_ptr(const Request *req, int *out_len);
const char *req_headers_ptr(const Request *req, int *out_len);
const char *req_body_ptr(const Request *req, int *out_len);

/* Find header value by name (case-insensitive). Returns pointer into buffer and sets out_len.
 * If not found returns NULL and out_len set to 0.
 * Header value is trimmed of leading/trailing spaces and not null-terminated.
 */
const char *req_header_get(const Request *req, const char *name, int *out_len);

/* Iterate headers: call callback for each header line name/value (offsets into buffer)
 * callback signature: int cb(const char *name_ptr, int name_len, const char *val_ptr, int val_len, void *ud)
 * Return 0 on success, negative on error, or callback return value if non-zero.
 */
int req_headers_iter(const Request *req,
                     int (*cb)(const char*,int,const char*,int,void*),
                     void *ud);



/* method string */
const char *req_method_str(const Request *req) {
    if (!req) return "UNSUPPORTED";
    switch (req->method) {
        case M_GET: return "GET";
        case M_POST: return "POST";
        case M_PUT: return "PUT";
        case M_DELETE: return "DELETE";
        case M_PATCH: return "PATCH";
        case M_OPTIONS: return "OPTIONS";
        case M_CONNECT: return "CONNECT";
        case M_TRACE: return "TRACE";
        case M_HEAD: return "HEAD";
        default: return "UNSUPPORTED";
    }
}

/* pointer+len helpers */
const char *req_uri_ptr(const Request *req, int *out_len) {
    if (!req || !req->buffer || req->uri_len <= 0) { if (out_len) *out_len = 0; return NULL; }
    if (out_len) *out_len = req->uri_len;
    return req->buffer + req->uri_start;
}

const char *req_query_ptr(const Request *req, int *out_len) {
    if (!req || !req->buffer || req->uri_query_len <= 0) { if (out_len) *out_len = 0; return NULL; }
    if (out_len) *out_len = req->uri_query_len;
    return req->buffer + req->uri_start + req->uri_len + 1; /* +1 skip '?' */
}

const char *req_version_ptr(const Request *req, int *out_len) {
    if (!req || !req->buffer || req->version_len <= 0) { if (out_len) *out_len = 0; return NULL; }
    if (out_len) *out_len = req->version_len;
    return req->buffer + req->version_start;
}

const char *req_headers_ptr(const Request *req, int *out_len) {
    if (!req || !req->buffer || req->headers_len <= 0) { if (out_len) *out_len = 0; return NULL; }
    if (out_len) *out_len = req->headers_len;
    return req->buffer + req->headers_start;
}

const char *req_body_ptr(const Request *req, int *out_len) {
    if (!req || !req->buffer || req->body_len <= 0) { if (out_len) *out_len = 0; return NULL; }
    if (out_len) *out_len = req->body_len;
    return req->buffer + req->body_start;
}

/* trim helpers */
static inline int is_lws(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }
static void trim_bounds(const char *buf, int *start, int *len) {
    int s = *start, l = *len;
    while (l > 0 && is_lws(buf[s])) { s++; l--; }
    while (l > 0 && is_lws(buf[s + l - 1])) { l--; }
    *start = s; *len = l;
}

/* case-insensitive compare for header name against buffer segment */
static int header_name_equals(const char *buf, int pos, int name_len, const char *name) {
    int nlen = (int)strlen(name);
    if (name_len != nlen) return 0;
    for (int i = 0; i < nlen; ++i) {
        char a = buf[pos + i];
        char b = name[i];
        if (a >= 'A' && a <= 'Z') a = a - 'A' + 'a';
        if (b >= 'A' && b <= 'Z') b = b - 'A' + 'a';
        if (a != b) return 0;
    }
    return 1;
}

/* find header value by name (first occurrence) */
const char *req_header_get(const Request *req, const char *name, int *out_len) {
    if (!req || !req->buffer || !name || req->headers_len <= 0) { if (out_len) *out_len = 0; return NULL; }
    int hdr_end = req->headers_start + req->headers_len; /* points to final '\n'+1 */
    int line = req->headers_start;
    while (line < hdr_end - 1) {
        /* find line end CRLF */
        int le = line;
        while (le < hdr_end - 1 && !(req->buffer[le] == '\r' && req->buffer[le+1] == '\n')) le++;
        if (le >= hdr_end - 1) break;
        /* locate ':' */
        int colon = -1;
        for (int p = line; p < le; ++p) if (req->buffer[p] == ':') { colon = p; break; }
        if (colon > 0) {
            int name_len = colon - line;
            if (header_name_equals(req->buffer, line, name_len, name)) {
                /* value starts after colon + optional spaces */
                int vs = colon + 1;
                while (vs < le && (req->buffer[vs] == ' ' || req->buffer[vs] == '\t')) vs++;
                int vlen = le - vs;
                /* trim trailing spaces */
                int tmp_s = vs; int tmp_len = vlen;
                trim_bounds(req->buffer, &tmp_s, &tmp_len);
                if (out_len) *out_len = tmp_len;
                return req->buffer + tmp_s;
            }
        }
        line = le + 2;
    }
    if (out_len) *out_len = 0;
    return NULL;
}

/* headers iterator */
int req_headers_iter(const Request *req,
                     int (*cb)(const char*,int,const char*,int,void*),
                     void *ud)
{
    if (!req || !req->buffer || !cb) return -1;
    int hdr_end = req->headers_start + req->headers_len;
    int line = req->headers_start;
    while (line < hdr_end - 1) {
        int le = line;
        while (le < hdr_end - 1 && !(req->buffer[le] == '\r' && req->buffer[le+1] == '\n')) le++;
        if (le >= hdr_end - 1) break;
        int colon = -1;
        for (int p = line; p < le; ++p) if (req->buffer[p] == ':') { colon = p; break; }
        if (colon > 0) {
            int name_s = line;
            int name_l = colon - name_s;
            int val_s = colon + 1;
            while (val_s < le && (req->buffer[val_s] == ' ' || req->buffer[val_s] == '\t')) val_s++;
            int val_l = le - val_s;
            int tmp_vs = val_s; int tmp_vl = val_l;
            trim_bounds(req->buffer, &tmp_vs, &tmp_vl);
            int cb_res = cb(req->buffer + name_s, name_l, req->buffer + tmp_vs, tmp_vl, ud);
            if (cb_res != 0) return cb_res;
        } else {
            /* header without colon — skip */
        }
        line = le + 2;
    }
    return 0;
}

void send_error(Request * req, int code);
void send_response(Request * req, char * response, unsigned long length);
void handle_transaction(Request * req);


#define READ_MAX_RETRIES 200
#define READ_TIMEOUT_SECS 5

/*
 * Read request-line, headers, and body into req->buffer.
 * - ssl: SSL connection
 * - req: Request pointer; req->buffer must be allocated with buffer_size
 * - buffer_size: size of req->buffer
 *
 * On success returns total bytes placed into req->buffer (req->total_read updated).
 * On error returns -1 (caller should cleanup and close connection).
 *
 * This function:
 *  - reads until "\r\n" for request-line
 *  - reads until "\r\n\r\n" for headers
 *  - parses headers in-place to find Content-Length (case-insensitive)
 *  - reads Content-Length bytes of body (if present), ensuring not to overflow buffer_size
 *
 * Note: It does not attempt to parse the method/URI/version beyond ensuring request-line ended.
 */

/* Helper: case-insensitive compare for method names in buffer starting at pos */
static int match_token(const char *buf, int buf_len, int pos, const char *token) {
    int tlen = (int)strlen(token);
    if (pos + tlen > buf_len) return 0;
    for (int i = 0; i < tlen; ++i) {
        if (tolower((unsigned char)buf[pos + i]) != tolower((unsigned char)token[i])) return 0;
    }
    return 1;
}

/* Parse method from buffer[0..reqline_len-1], set req->method and return index after method+space */
static int parse_method_from_buffer(Request *req) {
    if (!req || !req->buffer) return -1;
    int rl = req->reqline_len;
    /* check common methods */
    const struct { const char *name; Method m; } methods[] = {
        {"OPTIONS", M_OPTIONS}, {"DELETE", M_DELETE}, {"CONNECT", M_CONNECT},
        {"GET", M_GET}, {"PUT", M_PUT}, {"PATCH", M_PATCH},
        {"TRACE", M_TRACE}, {"HEAD", M_HEAD}, {"POST", M_POST}
    };
    /* find first space */
    int p = 0;
    while (p < rl && req->buffer[p] != ' ') p++;
    if (p >= rl) return -1;
    for (size_t i = 0; i < sizeof(methods)/sizeof(methods[0]); ++i) {
        if (p == (int)strlen(methods[i].name) && match_token(req->buffer, rl, 0, methods[i].name)) {
            req->method = methods[i].m;
            return p + 1; /* position after space */
        }
    }
    req->method = M_UNSUPPORTED;
    return p + 1;
}

/* ssl_write_all: writes full buffer, returns bytes written or -1 on fatal error */
int ssl_write_all(SSL *ssl, const char *buffer, int buffer_size) {
    int total_written = 0;
    int retries = 0;

    while (total_written < buffer_size) {
        int bytes_written = SSL_write(ssl, buffer + total_written, buffer_size - total_written);
        if (bytes_written > 0) {
            total_written += bytes_written;
            retries = 0;
            continue;
        }
        /* bytes_written <= 0: examine error */
        int err = SSL_get_error(ssl, bytes_written);
        if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
            /* wait for socket readiness with timeout */
            int fd = SSL_get_fd(ssl);
            if (fd >= 0) {
                fd_set wfds;
                FD_ZERO(&wfds);
                FD_SET(fd, &wfds);
                struct timeval tv = { READ_TIMEOUT_SECS, 0 };
                int sel = select(fd + 1, NULL, &wfds, NULL, &tv);
                if (sel <= 0) {
                    if (sel == 0) fprintf(stderr, "ssl_write_all: timeout waiting to write\n");
                    else perror("ssl_write_all: select");
                    return -1;
                }
                continue;
            } else {
                /* no fd; small sleep */
                if (++retries > READ_MAX_RETRIES) return -1;
                usleep(1000);
                continue;
            }
        } else if (err == SSL_ERROR_ZERO_RETURN) {
            /* connection closed cleanly by peer */
            return total_written;
        } else if (err == SSL_ERROR_SYSCALL) {
            perror("SSL_write syscall error");
            return -1;
        } else if (err == SSL_ERROR_SSL) {
            ERR_print_errors_fp(stderr);
            return -1;
        } else {
            fprintf(stderr, "ssl_write_all: unknown SSL error %d\n", err);
            return -1;
        }
    }
    return total_written;
}

/* ssl_read_request: read request-line, headers, detect content-length, read body if present.
 * Populates req->total_read and the various offset/length fields inside Request.
 * Returns >=0 total bytes read, -1 on fatal error.
 */
int ssl_read_request(SSL *ssl, Request *req, int buffer_size) {
    if (!req || !req->buffer) return -1;
    int total = 0;
    int retries = 0;
    int got_request_line = 0;
    int got_headers = 0;
    long content_length = -1;
    int i;

    req->buffer_size = buffer_size;
    req->total_read = 0;

    /* helper read loop with WANT handling and select timeout */
    for (;;) {
        /* Stage 1: read until request-line (CRLF) */
        while (!got_request_line) {
            if (total >= buffer_size) { fprintf(stderr, "buffer overflow reading request-line\n"); return -1; }
            int max_to_read = buffer_size - total;
            int r = SSL_read(ssl, req->buffer + total, max_to_read);
            if (r > 0) {
                total += r;
            } else if (r == 0) {
                /* peer closed */
                req->total_read = total;
                return total;
            } else {
                int err = SSL_get_error(ssl, r);
                if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
                    retries++;
                    if (retries > READ_MAX_RETRIES) { fprintf(stderr, "too many read retries\n"); return -1; }
                    int fd = SSL_get_fd(ssl);
                    if (fd >= 0) {
                        fd_set rfds;
                        FD_ZERO(&rfds);
                        FD_SET(fd, &rfds);
                        struct timeval tv = { READ_TIMEOUT_SECS, 0 };
                        int sel = select(fd + 1, &rfds, NULL, NULL, &tv);
                        if (sel <= 0) { if (sel == 0) fprintf(stderr, "read timeout\n"); else perror("select"); return -1; }
                        continue;
                    } else { usleep(1000); continue; }
                } else if (err == SSL_ERROR_SYSCALL) {
                    if (errno) perror("SSL_read syscall");
                    else fprintf(stderr, "SSL_read syscall (no errno)\n");
                    return -1;
                } else { ERR_print_errors_fp(stderr); return -1; }
            }

            /* search for CRLF */
            if (total >= 2) {
                for (i = 0; i <= total - 2; ++i) {
                    if (req->buffer[i] == '\r' && req->buffer[i+1] == '\n') {
                        got_request_line = 1;
                        break;
                    }
                }
            }
        }

        /* Stage 2: read headers until CRLFCRLF */
        while (!got_headers) {
            if (total >= buffer_size) { fprintf(stderr, "buffer overflow reading headers\n"); return -1; }
            /* check existing bytes for header terminator */
            if (total >= 4) {
                int found = 0;
                for (i = 3; i < total; ++i) {
                    if (req->buffer[i-3] == '\r' && req->buffer[i-2] == '\n' &&
                        req->buffer[i-1] == '\r' && req->buffer[i] == '\n') {
                        got_headers = 1;
                        found = 1;
                        break;
                    }
                }
                if (found) break;
            }

            int max_to_read = buffer_size - total;
            int r = SSL_read(ssl, req->buffer + total, max_to_read);
            if (r > 0) {
                total += r;
            } else if (r == 0) {
                req->total_read = total;
                return total;
            } else {
                int err = SSL_get_error(ssl, r);
                if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
                    retries++;
                    if (retries > READ_MAX_RETRIES) { fprintf(stderr, "too many read retries\n"); return -1; }
                    int fd = SSL_get_fd(ssl);
                    if (fd >= 0) {
                        fd_set rfds;
                        FD_ZERO(&rfds);
                        FD_SET(fd, &rfds);
                        struct timeval tv = { READ_TIMEOUT_SECS, 0 };
                        int sel = select(fd + 1, &rfds, NULL, NULL, &tv);
                        if (sel <= 0) { if (sel == 0) fprintf(stderr, "read timeout\n"); else perror("select"); return -1; }
                        continue;
                    } else { usleep(1000); continue; }
                } else if (err == SSL_ERROR_SYSCALL) {
                    if (errno) perror("SSL_read syscall");
                    else fprintf(stderr, "SSL_read syscall (no errno)\n");
                    return -1;
                } else { ERR_print_errors_fp(stderr); return -1; }
            }
        }

        break; /* headers read */
    }

    /* header_end index: 'i' points to index of final '\n' in the CRLFCRLF sequence */
    int header_end_idx = i;
    req->total_read = total;

    /* compute request-line len (first CRLF found earlier at position rl_end) */
    int rl_end = -1;
    for (int k = 0; k + 1 < total; ++k) {
        if (req->buffer[k] == '\r' && req->buffer[k+1] == '\n') { rl_end = k; break; }
    }
    if (rl_end < 0) { fprintf(stderr, "request-line end not found\n"); return -1; }
    req->reqline_start = 0;
    req->reqline_len = rl_end;

    /* Headers start after request-line CRLF */
    int hdr_start = rl_end + 2;
    req->headers_start = hdr_start;
    req->headers_len = (header_end_idx + 1) - hdr_start; /* include final '\n' */

    /* find Content-Length header (case-insensitive) inside headers */
    int line_start = hdr_start;
    while (line_start < hdr_start + req->headers_len - 1) {
        int line_end = line_start;
        while (line_end < total && !(req->buffer[line_end] == '\r' && req->buffer[line_end+1] == '\n')) line_end++;
        if (line_end >= total) break;
        /* find colon */
        int colon = -1;
        for (int p = line_start; p < line_end; ++p) if (req->buffer[p] == ':') { colon = p; break; }
        if (colon >= 0) {
            int name_len = colon - line_start;
            const char *target = "content-length";
            if (name_len == (int)strlen(target)) {
                int match = 1;
                for (int z = 0; z < name_len; ++z) {
                    char a = req->buffer[line_start + z];
                    if (a >= 'A' && a <= 'Z') a = a - 'A' + 'a';
                    if (a != target[z]) { match = 0; break; }
                }
                if (match) {
                    int vs = colon + 1;
                    while (vs < line_end && (req->buffer[vs] == ' ' || req->buffer[vs] == '\t')) vs++;
                    int vlen = line_end - vs;
                    if (vlen > 0) {
                        char tmp[64];
                        int cp = (vlen < (int)sizeof(tmp)-1) ? vlen : (int)sizeof(tmp)-1;
                        memcpy(tmp, req->buffer + vs, cp);
                        tmp[cp] = '\0';
                        content_length = atol(tmp);
                        if (content_length < 0) content_length = 0;
                    }
                }
            }
         // req->host = "web.opaq.co.il";
        }
        line_start = line_end + 2;
    }

    /* compute body start and read body if needed */
    req->body_start = header_end_idx + 1;
    req->body_len = req->total_read - req->body_start; /* may be 0 */

    if (content_length > 0) {
        /* ensure total buffer can hold full body_start+content_length */
        if ((long)req->body_start + content_length > buffer_size) {
            fprintf(stderr, "Content-Length too large for buffer\n");
            return -1;
        }
        if (req->body_len < (int)content_length) {
            int remaining = (int)(content_length - req->body_len);
            while (remaining > 0) {
                int max_to_read = buffer_size - req->total_read;
                if (max_to_read <= 0) { fprintf(stderr, "no space to read body\n"); return -1; }
                int r = SSL_read(ssl, req->buffer + req->total_read, max_to_read);
                if (r > 0) {
                    req->total_read += r;
                    req->body_len += r;
                    remaining -= r;
                    continue;
                } else if (r == 0) {
                    /* peer closed before full body */
                    return req->total_read;
                } else {
                    int err = SSL_get_error(ssl, r);
                    if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
                        int fd = SSL_get_fd(ssl);
                        if (fd >= 0) {
                            fd_set rfds;
                            FD_ZERO(&rfds);
                            FD_SET(fd, &rfds);
                            struct timeval tv = { READ_TIMEOUT_SECS, 0 };
                            int sel = select(fd + 1, &rfds, NULL, NULL, &tv);
                            if (sel <= 0) { if (sel == 0) fprintf(stderr, "read timeout\n"); else perror("select"); return -1; }
                            continue;
                        } else { usleep(1000); continue; }
                    } else if (err == SSL_ERROR_SYSCALL) {
                        if (errno) perror("SSL_read syscall"); else fprintf(stderr, "SSL_read syscall (no errno)\n");
                        return -1;
                    } else { ERR_print_errors_fp(stderr); return -1; }
                }
            }
        }
        /* now body_len should equal content_length */
        req->body_len = (int)content_length;
        req->total_read = req->body_start + req->body_len;
    }

    /* parse request-line into URI and version and method */
    /* request-line format: METHOD SP URI SP VERSION\r\n */
    /* we already have req->reqline_len and buffer[0..reqline_len-1] */
    int p = 0;
    /* find end of method (space) */
    while (p < req->reqline_len && req->buffer[p] != ' ') p++;
    if (p >= req->reqline_len) return -1;
    int after_method = p + 1;

    /* uri starts at after_method */
    int uri_s = after_method;
    int qlen = 0;
    int saw_q = 0;
    int uri_e = uri_s;
    while (uri_e < req->reqline_len && req->buffer[uri_e] != ' ') {
        if (!saw_q && req->buffer[uri_e] == '?') {
            /* mark length up to '?' */
            req->uri_len = uri_e - uri_s;
            saw_q = 1;
            uri_e++; /* include rest to count query length */
            continue;
        }
        if (saw_q) qlen++;
        uri_e++;
    }
    if (!saw_q) req->uri_len = uri_e - uri_s;
    req->uri_start = uri_s;
    req->uri_query_len = qlen;

    /* version starts after space at uri_e + 1 */
    if (uri_e >= req->reqline_len) return -1;
    int version_s = uri_e + 1;
    int version_len = req->reqline_len - version_s;
    if (version_len < 0) version_len = 0;
    req->version_start = version_s;
    req->version_len = version_len;

    /* method: find by reading buffer[0..p-1] */
    /* unify to parse_method_from_buffer which also sets method */
    req->reqline_len = rl_end;
    /* parse_method_from_buffer expects req->reqline_len set; ensure it is correct */
    /* we already have that; call parser */
    parse_method_from_buffer(req);

    return req->total_read;
}


char *request_body_copy(const Request *req, int *out_len) {
    if (!req || !req->buffer) {
        if (out_len) *out_len = 0;
        return NULL;
    }

    /* Validate body offsets */
    if (req->body_len <= 0) {
        if (out_len) *out_len = 0;
        return NULL; /* no body to copy */
    }
    if (req->body_start < 0 || req->body_start + req->body_len > req->total_read) {
        /* Invalid or truncated body */
        if (out_len) *out_len = 0;
        return NULL;
    }

    /* Allocate buffer (body_len + 1 for NUL) */
    char *buf = malloc((size_t)req->body_len + 1);
    if (!buf) {
        if (out_len) *out_len = 0;
        return NULL;
    }

    /* Copy the body bytes and NUL-terminate */
    memcpy(buf, req->buffer + req->body_start, (size_t)req->body_len);
    buf[req->body_len] = '\0';

    if (out_len) *out_len = req->body_len;
    return buf;
}

void close_socket(Request * req){
    close(req->con->sock);
    free(req->con);
}

void terminate_con(Request * req){
    SSL_shutdown(req->con->ssl);
    SSL_free(req->con->ssl);
    close_socket(req);
    free(req->buffer);
    free(req);
}

void * handle_client(void *arg) {
     //fprintf(stdout,"Handle_client\n");

    Request * req = malloc(sizeof(Request));
    req->con = (connection_t *) arg;
   
    req->buffer=malloc(BUFFER_SIZE * sizeof(char));
    
    

    inet_ntop(AF_INET, &req->con->address.sin_addr, req->ip, INET_ADDRSTRLEN);
    
    req->con->ssl = SSL_new(req->con->ctx);
    if (!req->con->ssl) {
        perror("Failed to create SSL structure");
        ERR_print_errors_fp(stderr);
        // close_socket(req);
        terminate_con(req);
        return NULL;
    }

    SSL_set_fd(req->con->ssl, req->con->sock);

    if (SSL_accept(req->con->ssl) <= 0) {
        perror("SSL accept failed");
        ERR_print_errors_fp(stderr);
        terminate_con(req);
        return NULL;
    }

    
    /* --- begin: read+parse request (place after SSL_set_fd and SSL_accept) --- */

int rr = ssl_read_request(req->con->ssl, req, BUFFER_SIZE);
if (rr < 0) {
    /* fatal read error: terminate and return */
    fprintf(stderr, "ssl_read_request failed\n");
    terminate_con(req);
    return NULL;
}
if (rr == 0) {
    /* peer closed without sending data */
    fprintf(stderr, "Client closed during read\n");
    terminate_con(req);
    return NULL;
}

/* At this point ssl_read_request populated:
   req->total_read, req->reqline_len, req->headers_start, req->headers_len,
   req->body_start, req->body_len
   Now populate method, uri, version offsets as done in the merged example.
*/

/* Parse request-line fields (METHOD SP URI SP VERSION\r\n) */
int rl_end = req->reqline_len;             /* index of '\r' in first CRLF */
if (rl_end <= 0 || rl_end >= req->total_read) {
    fprintf(stderr, "invalid request-line length\n");
    terminate_con(req);
    return NULL;
}

/* find method end (space) */
int p = 0;
while (p < rl_end && req->buffer[p] != ' ') p++;
if (p >= rl_end) {
    fprintf(stderr, "malformed request-line (no space after method)\n");
    terminate_con(req);
    return NULL;
}

/* set URI start (after method + space) */
int uri_s = p + 1;
int uri_e = uri_s;
int saw_q = 0;
int qlen = 0;

/* scan URI until space */
while (uri_e < rl_end && req->buffer[uri_e] != ' ') {
    if (!saw_q && req->buffer[uri_e] == '?') {
        req->uri_len = uri_e - uri_s; /* length of path before ? */
        saw_q = 1;
        uri_e++; /* continue to count query chars */
        continue;
    }
    if (saw_q) qlen++;
    uri_e++;
}
if (!saw_q) req->uri_len = uri_e - uri_s;
req->uri_start = uri_s;
req->uri_query_len = qlen;

/* version starts after the space following URI */
if (uri_e >= rl_end) {
    fprintf(stderr, "malformed request-line (no space before version)\n");
    terminate_con(req);
    return NULL;
}
int version_s = uri_e + 1;
int version_len = rl_end - version_s;
if (version_len < 0) version_len = 0;
req->version_start = version_s;
req->version_len = version_len;

/* Set request-line fields (reqline_start is 0) */
req->reqline_start = 0;
req->reqline_len = rl_end;

/* Determine Method enum (fast match against known methods) */
req->method = M_UNSUPPORTED;
if (p == 3 && (match_token(req->buffer, req->reqline_len, 0, "GET"))) req->method = M_GET;
else if (p == 4 && (match_token(req->buffer, req->reqline_len, 0, "POST"))) req->method = M_POST;
else if (p == 3 && (match_token(req->buffer, req->reqline_len, 0, "PUT"))) req->method = M_PUT;
else if (p == 6 && (match_token(req->buffer, req->reqline_len, 0, "DELETE"))) req->method = M_DELETE;
else if (p == 5 && (match_token(req->buffer, req->reqline_len, 0, "PATCH"))) req->method = M_PATCH;
else if (p == 7 && (match_token(req->buffer, req->reqline_len, 0, "OPTIONS"))) req->method = M_OPTIONS;
else if (p == 7 && (match_token(req->buffer, req->reqline_len, 0, "CONNECT"))) req->method = M_CONNECT;
else if (p == 5 && (match_token(req->buffer, req->reqline_len, 0, "TRACE"))) req->method = M_TRACE;
else if (p == 4 && (match_token(req->buffer, req->reqline_len, 0, "HEAD"))) req->method = M_HEAD;

/* Sanity checks */
if (req->uri_len < 0 || req->uri_len > MAX_URI_LENGTH || req->uri_query_len > MAX_URI_LENGTH) {
    send_error(req, 414);
    /* send_error will call terminate_con(req) */
    return NULL;
}
//Starting Services...
//itamar@vm:~/Workspace/www/pages$ su
/* Now req contains offsets only. Example usage below:
   - req->buffer + req->uri_start, length req->uri_len for path
   - req->buffer + req->uri_start + req->uri_len + 1, length req->uri_query_len for query (if >0)
   - req->buffer + req->version_start, length req->version_len for version string
   - headers block at req->buffer + req->headers_start length req->headers_len
   - body at req->buffer + req->body_start length req->body_len
*/
printf("%s\n", req->buffer);
/* You can now call your handler/dispatcher that expects Request populated */
//  fprintf(stdout,"Handle IN\n");
handle_transaction(req);
//  fprintf(stdout,"Handle OUT\n");

/* After handle_transaction returns, send response or ensure it sent one.
   If you send here, do it then terminate connection exactly once.
   Example: send a minimal 200 OK if not already sent by handler.
*/


/* Clean termination (single place to free SSL/socket and req buffer) */
// terminate_con(req);
return NULL;

/* --- end block --- */

}

void send_error(Request * req, int code){
    char out[1024];
   
    char * body = getStatus(code);
    
    sprintf(out, "HTTP/1.1 %s\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s", getStatus(code), strlen(body), body);
    send_response(req, out, strlen(out));
    // terminate_con(req);
    
    //fprintf(stdout,"\e[0;96m%s\e[0m: \e[0;95m%s\e[0m \e[1;91m%d\e[0m %s\n",t.ip, method_to_str(t.method), code, t.uri);
}

void send_response(Request * req, char * response, unsigned long length){
    if (ssl_write_all(req->con->ssl, response, length) <= 0) {
        perror("SSL write failed");
        ERR_print_errors_fp(stderr);
        
    }
}

typedef struct {
    int uri_length;
    int query_length;
    int response_length;
    int request_body_length;
    char time[64];
    char uri[MAX_URI_LENGTH];
    char query[MAX_URI_LENGTH];
    char response[HEADER_BUFFER_SIZE];
    char headers[HEADER_BUFFER_SIZE];
    Scope scope;
    char * request_body;
    Node * result;
    char * host;
    FileInfo file;
} Exchange;

void handle_transaction(Request * t){ 
    
    Exchange x = (Exchange){
        .uri_length = 0,
        .query_length = 0,
        .response_length = 0,
        .uri_length = 0,
        .request_body_length = 0,
    };
    memset( x.time, '\0', sizeof(char) * 64 );
    memset( x.uri, '\0', sizeof(char) * MAX_URI_LENGTH );
    memset( x.query, '\0', sizeof(char) * MAX_URI_LENGTH );
    memset( x.response, '\0', sizeof(char) * HEADER_BUFFER_SIZE );

    
    x.scope.current.parent = NULL;
    x.scope.global.classes = create_new_node_map();
    x.scope.current.functions = create_new_node_map();
    x.scope.current.vars = create_new_node_map();

    
    set_node(x.scope.current.vars, "ip", new_node(.type=LITERAL, .literal.type=STRING, .literal.str=t->ip));
    set_node(x.scope.current.vars, "method", new_node(.type=LITERAL, .literal.type=STRING, .literal.str=method_to_str(t->method)));

    Node * override_flag_ = new_node(.type=LITERAL, .literal.type=INT, .literal.num=0);
    
//    set_node(x.scope.current.vars, "Host", new_node(.type=LITERAL, .literal.type=STRING, .literal.str=t->Host));
    set_node(x.scope.current.vars, "overrideFlag", override_flag_);
    set_node(x.scope.current.vars, "overrideData", new_node(.type=LITERAL, .literal.type=STRING, .literal.str=""));
    set_node(x.scope.current.vars, "overrideType", new_node(.type=LITERAL, .literal.type=STRING, .literal.str=""));
    
    time_t now = time(NULL);
    // struct tm *tn = localtime(&now);
    strftime(x.time, sizeof(x.time), "%d-%m-%Y %H:%M:%S", localtime(&now));
    set_node(x.scope.current.vars, "time", new_node(.type=LITERAL, .literal.type=STRING, .literal.str=x.time));
    

    
    char * ptr = (char *)req_uri_ptr(t, &x.uri_length);
    //char * __uri = (char *)malloc(sizeof(char) * (x.uri_length + 1));
    strncpy(x.uri, ptr, x.uri_length);
    // __uri[uri_length] = '\0';
    set_node(x.scope.current.vars, "uri", new_node(.type=LITERAL, .literal.type=STRING, .literal.str=x.uri));

    // char * __query = ""; //malloc(sizeof(char) * (x.query_length + 1));
// ("after malloc\n");
    ptr = (char *)req_query_ptr(t, &x.query_length);
    strncpy(x.query, ptr, x.query_length);
//    __query[x.query_length] = '\0';
// ("before set\n");
    set_node(x.scope.current.vars, "query", new_node(.type=LITERAL, .literal.type=STRING, .literal.str=x.query));
// ("after query");
    // x.request_body_length = 0;
    x.request_body = request_body_copy(t, &x.request_body_length);
    set_node(x.scope.current.vars, "body", new_node(.type=LITERAL, .literal.type=STRING, .literal.str=( (x.request_body && x.request_body_length > 0) ? x.request_body : "")));


// ("before traverse\n");
    
    for (size_t i = 0; i < program->statement.block.count; i++) {
      x.result = traverse(program->statement.block.statements[i], 0, &x.scope);  // ✅ each statement is traversed individually
      if(x.result->type == ERROR){
        // override_flag->literal.num = 500;
        print_error(x.result, itlc);
        break;

      }
    }
//("after traverse\n");
    Node * override_flag = get_node(x.scope.current.vars, "overrideFlag");
    Node * override_data = get_node(x.scope.current.vars, "overrideData");
    Node * override_type = get_node(x.scope.current.vars, "overrideType");
    //x.response = calloc(1024, sizeof(char));
    // x.response =  calloc(HEADER_BUFFER_SIZE, sizeof(char));
	//("IS FILE\n");
    if(override_flag->literal.num == 0 && t->method == M_GET){  // default behavior

        

        if((strcmp(x.uri, "/") == 0 || strcmp(x.uri, "/index.html") == 0) && is_file("/index.html") > -1){
            // ("DEFAULT INDEX\n");

            x.file = get_file("/index.html");
            sprintf(x.response, "HTTP/1.1 %s\r\nContent-Length: %zu\r\nContent-Type: %s\r\n\r\n", getStatus(200), x.file.size, "text/html");
            send_response(t, x.response, strlen(x.response));
            send_response(t, x.file.data, x.file.size);
            
        }else if( is_file(x.uri) > -1){

            x.file = get_file(x.uri);
            sprintf(x.response, "HTTP/1.1 %s\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: %zu\r\nContent-Type: %s\r\n\r\n", getStatus(200), x.file.size, get_mime_type(x.uri));
            
            send_response(t, x.response, strlen(x.response));
            send_response(t, x.file.data, x.file.size);

        }else{


            sprintf(x.response, "HTTP/1.1 %s\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: %zu\r\nContent-Type: %s\r\n\r\n", getStatus(404), strlen(getStatus(404)), get_mime_type(".txt"), getStatus(404));
            send_response(t, x.response, strlen(x.response));
            
        }
    }else if(override_flag->literal.num == 600){
        if(is_file(override_data->literal.str) > -1){

            x.file = get_file(override_data->literal.str);
            sprintf(x.response, "HTTP/1.1 %s\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: %d\r\nContent-Type: %s\r\n\r\n", getStatus(200), x.file.size, get_mime_type(override_data->literal.str));
            
            send_response(t, x.response, strlen(x.response));
            send_response(t, x.file.data, x.file.size);

        }else{
            send_error(t, 404); //not found
        }
    }else{
        sprintf(x.response, "HTTP/1.1 %s\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: %d\r\nContent-Type: %s\r\n\r\n", getStatus((int)(long int)override_flag->literal.num), strlen(override_data->literal.str), get_mime_type( override_type->literal.str) ); //get_mime_type(t.uri));
        
        x.response_length = strlen(x.response);
        
        send_response(t, x.response, x.response_length);
        send_response(t, override_data->literal.str, strlen(override_data->literal.str));

    }


    // destroy_nodemap(x.scope.global.classes);
    // destroy_nodemap(x.scope.current.vars);
    // destroy_nodemap(x.scope.current.functions);
    // destroy_scope(x.scope);
    // cleanup(x);
    if(override_flag)
        free(override_flag);
    if(override_data)
        free(override_data);
    if(override_type)
        free(override_type);
    if(x.request_body)
        free(x.request_body);
    terminate_con(t);
    program = parse_program(tokens, itlc);
}

#endif


