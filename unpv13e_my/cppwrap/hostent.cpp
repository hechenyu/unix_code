#include <arpa/inet.h>
#include <netdb.h>

#include <stdlib.h>
#include <stdio.h>

#include <system_error>
#include <string>
#include <iostream>

enum class gethostbyname_errc {
	success = 0,
	host_not_found = HOST_NOT_FOUND,
	try_again = TRY_AGAIN,
	no_recovery = NO_RECOVERY,
	no_data = NO_DATA,
	unknown = -1,
};

namespace std {
template <>
struct is_error_condition_enum<gethostbyname_errc>: public true_type {
};
}

class gethostbyname_category: public std::error_category {
public:
	virtual const char *name() const noexcept { return "gethostbyname"; }
	virtual std::error_condition default_error_condition(int ev) const noexcept {
		switch (ev) {
		case 0:
			return std::error_condition(gethostbyname_errc::success);
		case HOST_NOT_FOUND:
			return std::error_condition(gethostbyname_errc::host_not_found);
		case TRY_AGAIN:
			return std::error_condition(gethostbyname_errc::try_again);
		case NO_RECOVERY:
			return std::error_condition(gethostbyname_errc::no_recovery);
		case NO_DATA:
			return std::error_condition(gethostbyname_errc::no_data);
		default:
			return std::error_condition(gethostbyname_errc::unknown);
		}
	}

	virtual std::string message(int ev) const {
		return hstrerror(ev);
	}
} gethostbyname_category_;

std::error_condition make_error_condition(gethostbyname_errc e) {
	return std::error_condition(static_cast<int>(e), gethostbyname_category_);
}

struct hostent *Gethostbyname(const char *name) {
	struct hostent *hptr = gethostbyname(name);
	if (hptr == NULL) {
		throw std::system_error(
			std::error_code(h_errno, gethostbyname_category_),
			std::string("gethostbyname error for host: ")+name);
	}
	return hptr;
}

int
main(int argc, char **argv)
{
	char			*ptr, **pptr;
	char			str[INET_ADDRSTRLEN];
	struct hostent	*hptr;

	while (--argc > 0) {
		ptr = *++argv;
		try {
			hptr = Gethostbyname(ptr);
		} catch (const std::exception &e) {
			std::cout << e.what() << std::endl;
			continue;
		}
		printf("official hostname: %s\n", hptr->h_name);

		for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
			printf("\talias: %s\n", *pptr);

		switch (hptr->h_addrtype) {
		case AF_INET:
			pptr = hptr->h_addr_list;
			for ( ; *pptr != NULL; pptr++)
				printf("\taddress: %s\n",
					inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
			break;

		default:
			perror("unknown address type");
			break;
		}
	}
	exit(0);
}
