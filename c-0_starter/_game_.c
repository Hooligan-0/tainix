/*
 * MIT License
 *
 * Copyright (c) 2024 Gwenael Saint-Genest <gwen@cowlab.fr>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#define JSMN_STRICT
#include "_jsmn_.h"
#include "_game_.h"

extern char  tainix_key[]; /* Key must be defined into each app */

static char  url_start[] = "https://tainix.fr/api/games/start";
static char  url_resp[]  = "https://tainix.fr/api/games/response";
static char  engine_code[64] = "";
static char *challenge_data  = 0;

static jsmn_parser  parser;
static jsmntok_t   *tokens = 0;

static int    cache_load(void);
static int    decode(void);
static char  *encode_base64(char *src);
static int    find_token(char *key, int idx_start, int pos_max);
static int    server_load(void);
static int    server_send(char *token, char *response);
static size_t server_writemem_cb(void *contents, size_t size, size_t nmemb, void *userp);

/**
 * @brief Load the input data of a challenge (from server or cache)
 *
 * @param engne_code Identifier of the challenge to load
 * @return integer Zero is returned on success, other values are error
 */
int game_get_challenge(char *code)
{
	int result = 0;

	memset(engine_code, 0, 64);
	if (code == 0)
	{
		fprintf(stderr, "GAME ERROR: Engine code not defined\n");
		return(-1);
	}
	if (strlen(code) > 63)
	{
		fprintf(stderr, "GAME ERROR: Engine code too long :(\n");
		return(-2);
	}
	strncpy(engine_code, code, 63);

	/* 1) Try to load challenge from cache (local file) */
	printf(" - Try to load challenge from cache ... ");
	result = cache_load();
	if (result == 0)
	{
		if (decode() == 0)
			printf("Success :)\n");
		else
			result = -1;
	}
	else
		printf("Failed (%d)\n", result);

	if (result == 0)
		goto finish;

	/* 2) Try to load challenge from server */
	printf(" - Try to load challenge from server ... ");
	result = server_load();
	if (result == 0)
	{
		if (decode() == 0)
			printf("Success :)\n");
		else
			result = -1;
	}
	else
		printf("Failed (%d)\n", result);

finish:
	printf("\n");
	return(result);
}

/**
 * @brief Get the value of one challenge input variabled identified by his name
 *
 * @param key Name of the input variable
 * @return char* Pointer to a string with the result (or NULL)
 */
char *game_input(char *key)
{
	int i_idx, i;
	int result_len;
	char *result;

	/* Challenge data must be loaded first */
	if (tokens == 0)
		return(0);
	/* Root element of challenge data must be an object ! */
	if (tokens[0].type != JSMN_OBJECT)
		return(0);

	/* Search the "input" field into challenge data ... */
	i_idx = find_token("input", 1, tokens[0].end);
	if (i_idx < 0)
		return(0);
	/* ... then, search the specified key into inputs */
	i = find_token(key, i_idx+1, tokens[i_idx+1].end);
	if (i < 0)
		return(0);

	int r_start = tokens[i+1].start;
	if (challenge_data[r_start] == '"')
		r_start++;
	result_len = (tokens[i+1].end - r_start);

	/* Allocate memory for result */
	result = (char *)malloc(result_len + 1);
	/* Clear memory :) */
	memset(result, 0, result_len + 1);
	/* Extract/copy requested key into result */
	memcpy(result, challenge_data + r_start, result_len);

	return(result);
}

/**
 * @brief Send response to the server and display result
 *
 * @param response String with the response to send
 * @return integer Zero is returned is response has been successfully submited
 */
int game_send_response(char *response)
{
	const char header[] = "{\"data\": ";
	const char tail[]   = "}";
	char *json, *b64, *tok;
	int   jlen, tok_len, tok_start;
	int   idx;
	int   result = 0;

	/* Sanity check */
	if (response == 0)
		return(-1);

	json = 0;
	b64  = 0;

	/* 1) Get the _game token_ from received challenge */

	/* Search token */
	idx = find_token("token", 1, tokens[0].end);
	if (idx < 0)
		return(0);
	tok_start = tokens[idx+1].start;
	if (challenge_data[tok_start] == '"')
		tok_start++;
	tok_len = (tokens[idx+1].end - tok_start);
	/* Allocate memory for token string */
	tok = (char *)malloc(tok_len + 1);
	if (tok == 0)
		goto err_mem;
	/* Clear memory :) */
	memset(tok, 0, tok_len + 1);
	/* Extract/copy requested key into result */
	memcpy(tok, challenge_data + tok_start, tok_len);

	/* 2) Make a JSON string with response to send */

	jlen = strlen(header) + strlen(response) + strlen(tail);
	json = (char *)malloc(jlen + 2);
	if (json == 0)
		goto err_mem;

	strcpy(json, header);
	strcat(json, response);
	strcat(json, tail);

	/* 3) Encode response and send to server */

	b64 = encode_base64(json);

	server_send(tok, b64);

	result = 0;
	goto finish;

err_mem:
	result = -1;
finish:
	if (b64)
		free(b64);
	if (json)
		free(json);
	return(result);
}

/* -------------------------------------------------------------------------- */
/* --                                                                      -- */
/* --                       Data cache  (local file)                       -- */
/* --                                                                      -- */
/* -------------------------------------------------------------------------- */

/**
 * @brief Load challenge data from cached file
 *
 * @return integer Zero is returned on success, other values are errors
 */
static int cache_load(void)
{
	char   cache_filename[80];
	FILE  *f;
	fpos_t pos;
	int result = 0;
	int len;

	/* Open local cache file */
	sprintf(cache_filename, "%s.cache", engine_code);
	f = fopen(cache_filename, "r");
	if (f == 0)
		return(-1);

	/* Try to determine file size */
	if (fseek(f, 0, SEEK_END))
		goto err_size;
	if (fgetpos(f, &pos))
		goto err_size;
	/* If file is empty, failed */
	if (pos.__pos == 0)
		goto err_size;

	/* Allocate memory for challenge data */
	challenge_data = malloc(pos.__pos + 1);
	if (challenge_data == 0)
		goto err_mem;
	/* Clear memory (an extra and optional security) */
	memset(challenge_data, 0, pos.__pos + 1);

	/* Read file content */
	if (fseek(f, 0, SEEK_SET))
		goto err_size;
	len = fread(challenge_data, 1, pos.__pos, f);
	if (len != pos.__pos)
		goto err_read;

	/* Try to decode JSON challenge */
	jsmn_init(&parser);
	if (jsmn_parse(&parser, challenge_data, pos.__pos, 0, 0) < 0)
		goto err_read;

	fclose(f);
	return(result);

err_size: /* Empty file or Failed to determine file size */
	result = -2;
	goto finish;
err_mem: /* Failed to allocate memory */
	result = -3;
	goto finish;
err_read: /* Failed to read file content */
	result = -4;
	goto finish;
finish:
	if (f)
		fclose(f);
	if (tokens)
	{
		free(tokens);
		tokens = 0;
	}
	if (challenge_data)
	{
		free(challenge_data);
		challenge_data = 0;
	}
	return(result);
}

/**
 * @brief Write challenge data into local file
 *
 * @return integer Zero is returned on success, other values are errors
 */
int cache_write(void)
{
	char cache_filename[80];
	FILE *fcache;
	int len;

	/* If the engine identifier is not available */
	if (strlen(engine_code) == 0)
		return(-1);
	/* Challenge data must be available */
	if ((challenge_data == 0) || (strlen(challenge_data) == 0))
		return(-2);

	sprintf(cache_filename, "%s.cache", engine_code);
	fcache = fopen(cache_filename, "w");
	if (fcache == 0)
		return(-3);

	len = strlen(challenge_data);
	fwrite(challenge_data, 1, len, fcache);
	fclose(fcache);

	return(0);
}

/* -------------------------------------------------------------------------- */
/* --                                                                      -- */
/* --                        Encoders and Decoders                         -- */
/* --                                                                      -- */
/* -------------------------------------------------------------------------- */

/**
 * @brief Decode JSON packet from received text string
 *
 * @return integer Zero is returned on success, other values are errors
 */
static int decode(void)
{
	int len;
	unsigned int i;
	
	len = strlen(challenge_data);
	
	for (i = 16; ; i += 16)
	{
		/* Allocate memory required to save tokens */
		tokens = (jsmntok_t *)malloc(sizeof(jsmntok_t) * i);
		if (tokens == 0)
			goto err_mem;
		/* Call JSON parser to fill array of tokens */
		jsmn_init(&parser);
		jsmn_parse(&parser, challenge_data, len, tokens, i);

		/* If token buffer was large enough ... finish :) */
		if (parser.toknext < i)
			break;
		/* Buffer was too small, free the current buffer and restart */
		free(tokens);
		tokens = 0;
	}
	return(0);
err_mem:
	return(-1);
}

/**
 * @brief Encode buffer into Base64 text string
 *
 * @param src Pointer to a text string with input data to encode
 * @return char* Pointer to a newly allocated buffer with base64 string
 */
static char *encode_base64(char *src)
{
	const int mod_table[] = {0, 2, 1};
	char *b64;
	int   src_len, b64_len;
	unsigned long a,b,c, triple;
	int i,j;
	const char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	                               'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	                               'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	                               'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	                               'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	                               'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	                               'w', 'x', 'y', 'z', '0', '1', '2', '3',
	                               '4', '5', '6', '7', '8', '9', '+', '/'};

	/* Sanity check */
	if (src == 0)
		return(0);

	/* Compute length of input and output data */
	src_len = strlen(src);
	b64_len = 4 * ((src_len + 2) / 3);

	/* Allocate a buffer to store base64 encoded result */
	b64 = (char *)malloc(b64_len + 1);
	if (b64 == 0)
		goto err_mem;
	/* Clear memory :) */
	memset(b64, 0, b64_len + 1);

	for (i = 0, j = 0; i < src_len;)
	{
		a = i < src_len ? (unsigned char)src[i++] : 0;
		b = i < src_len ? (unsigned char)src[i++] : 0;
		c = i < src_len ? (unsigned char)src[i++] : 0;
		triple = (a << 0x10) + (b << 0x08) + c;
		b64[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
		b64[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
		b64[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
		b64[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
	}
	for (i = 0; i < mod_table[src_len % 3]; i++)
		b64[b64_len - 1 - i] = '=';

	return(b64);
err_mem:
	return(0);
}

/**
 * @brief Search a specific key/value pair into JSON of the challenge
 *
 * @param key Pointer to a string with searched key
 * @param idx_start Start index into json token array for search
 * @param pos_max   Maximal offset into input data (in bytes !)
 * @return integer  Index of the element into JSON array
 */
static int find_token(char *key, int idx_start, int pos_max)
{
	int match_len, match_name;
	int bref;
	int i;

	bref = 0;

	for (i = idx_start; i < (int)parser.toknext; i++)
	{
		if (tokens[i].start < bref)
			continue;
		if (tokens[i].end > pos_max)
			return(-1);

		if (tokens[i].type == JSMN_STRING)
		{
			size_t tlen = (tokens[i].end - tokens[i].start);
			match_len  = (tlen == strlen(key)) ? 1 : 0;
			match_name = (strncmp(key, challenge_data + tokens[i].start, tlen) == 0) ? 1 : 0;
			if (match_len && match_name)
				return(i);
			else
				bref = tokens[i+1].end;
		}
	}
	return(-1);
}

/* -------------------------------------------------------------------------- */
/* --                                                                      -- */
/* --                      Communication with server                       -- */
/* --                                                                      -- */
/* -------------------------------------------------------------------------- */

/**
 * @brief Load challenge data from Tainix server
 *
 * @return integer Zero is returned on success, other values are errors
 */
static int server_load(void)
{
	CURL *curl_hnd;
	CURLcode res;
	char *url = 0;
	int   len;
	int   result = -1;

	curl_hnd = curl_easy_init();
	if (curl_hnd == 0)
		return(-1);

	/* Compute lenght of URL */
	len  = strlen(url_start);
	len += strlen(tainix_key)  + 1;
	len += strlen(engine_code) + 1;
	/* Allocate memory for url */
	url = malloc(len + 1);
	if (url == 0)
		goto err_mem;
	/* Make URL string */
	sprintf(url, "%s/%s/%s", url_start, tainix_key, engine_code);

	curl_easy_setopt(curl_hnd, CURLOPT_URL, url);
	curl_easy_setopt(curl_hnd, CURLOPT_WRITEFUNCTION, server_writemem_cb);
	/* Perform the request, res will get the return code */
	res = curl_easy_perform(curl_hnd);
	if(res != CURLE_OK)
		goto failed;

	/* Verify received data : should be valid JSON */
	len = strlen(challenge_data);
	jsmn_init(&parser);
	if (jsmn_parse(&parser, challenge_data, len, 0, 0) < 0)
		goto failed;

	/* Save a copy of the received data into cache file */
	if (cache_write() < 0)
		goto failed;

	result = 0;
	goto finish;

err_mem:
failed:
	result = -1;
finish:
	if (url)
		free(url);
	if (curl_hnd)
	{
		curl_easy_cleanup(curl_hnd);
		curl_global_cleanup();
	}
	return(result);
}

/**
 * @brief Send request with submited response
 *
 * @param token Pointer to a text string with game token
 * @param response Pointer to a text string with Base64 response
 */
static int server_send(char *token, char *response)
{
	CURL *curl_hnd;
	CURLcode res;
	char *url = 0;
	int   len;

	curl_hnd = curl_easy_init();
	if (curl_hnd == 0)
		return(-1);

	/* Compute lenght of URL */
	len  = strlen(url_resp);
	len += strlen(token)    + 1;
	len += strlen(response) + 1;
	/* Allocate memory for url */
	url = malloc(len + 1);
	if (url == 0)
		goto err_mem;
	/* Make URL string */
	sprintf(url, "%s/%s/%s", url_resp, token, response);

	curl_easy_setopt(curl_hnd, CURLOPT_URL, url);
	/* Perform the request, res will get the return code */
	res = curl_easy_perform(curl_hnd);
	if(res != CURLE_OK)
		goto failed;
	
	printf("\n");
	return(0);
err_mem:
failed:
	return(-1);
}

/**
 * @brief Callback used by libcurl when data are received from server
 *
 */
static size_t server_writemem_cb(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	int len;

	(void)userp;

	if (challenge_data == 0)
	{
		/* Allocate memory to save received content */
		challenge_data = malloc(realsize + 1);
		if (challenge_data == 0)
			return(0);
		/* Clear memory (extra and optional security) */
		memset(challenge_data, 0, realsize + 1);
		/* Copy received datas */
		memcpy(challenge_data, contents, realsize);
	}
	else
	{
		len = strlen(challenge_data);
		challenge_data = realloc(challenge_data, len + realsize + 1);
		if (challenge_data == 0)
			return(0);
		/* Clear memory (extra and optional security) */
		memset(challenge_data + len, 0, realsize + 1);
		/* Copy received data */
		memcpy(challenge_data + len, contents, realsize);
	}

	return(realsize);
}
/* EOF */
