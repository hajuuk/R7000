#ifndef __LIBIPSEC_H_
#define __LIBIPSEC_H_ 1

char *ipsec_dump_policy(caddr_t policy, char *delimiter);
void ipsec_hexdump(caddr_t buf, int len);
int  ipsec_get_policylen(caddr_t policy);
caddr_t ipsec_set_policy(char *msg, int msglen);
extern const char *ipsec_strerror(void);

struct sadb_msg;
void kdebug_sadb(struct sadb_msg *base);

#endif
