#include "memcached_rb.h"

const char *MEMCACHED_BEHAVIOR_NAMES[] = {
	"MEMCACHED_BEHAVIOR_NO_BLOCK",
	"MEMCACHED_BEHAVIOR_TCP_NODELAY",
	"MEMCACHED_BEHAVIOR_HASH",
	"MEMCACHED_BEHAVIOR_KETAMA",
	"MEMCACHED_BEHAVIOR_SOCKET_SEND_SIZE",
	"MEMCACHED_BEHAVIOR_SOCKET_RECV_SIZE",
	"MEMCACHED_BEHAVIOR_CACHE_LOOKUPS",
	"MEMCACHED_BEHAVIOR_SUPPORT_CAS",
	"MEMCACHED_BEHAVIOR_POLL_TIMEOUT",
	"MEMCACHED_BEHAVIOR_DISTRIBUTION",
	"MEMCACHED_BEHAVIOR_BUFFER_REQUESTS",
	"MEMCACHED_BEHAVIOR_USER_DATA",
	"MEMCACHED_BEHAVIOR_SORT_HOSTS",
	"MEMCACHED_BEHAVIOR_VERIFY_KEY",
	"MEMCACHED_BEHAVIOR_CONNECT_TIMEOUT",
	"MEMCACHED_BEHAVIOR_RETRY_TIMEOUT",
	"MEMCACHED_BEHAVIOR_KETAMA_WEIGHTED",
	"MEMCACHED_BEHAVIOR_KETAMA_HASH",
	"MEMCACHED_BEHAVIOR_BINARY_PROTOCOL",
	"MEMCACHED_BEHAVIOR_SND_TIMEOUT",
	"MEMCACHED_BEHAVIOR_RCV_TIMEOUT",
	"MEMCACHED_BEHAVIOR_SERVER_FAILURE_LIMIT",
	"MEMCACHED_BEHAVIOR_IO_MSG_WATERMARK",
	"MEMCACHED_BEHAVIOR_IO_BYTES_WATERMARK",
	"MEMCACHED_BEHAVIOR_IO_KEY_PREFETCH",
	"MEMCACHED_BEHAVIOR_HASH_WITH_PREFIX_KEY",
	"MEMCACHED_BEHAVIOR_NOREPLY",
	"MEMCACHED_BEHAVIOR_USE_UDP",
	"MEMCACHED_BEHAVIOR_AUTO_EJECT_HOSTS",
	"MEMCACHED_BEHAVIOR_NUMBER_OF_REPLICAS",
	"MEMCACHED_BEHAVIOR_RANDOMIZE_REPLICA_READ",
	"MEMCACHED_BEHAVIOR_CORK",
	"MEMCACHED_BEHAVIOR_TCP_KEEPALIVE",
	"MEMCACHED_BEHAVIOR_TCP_KEEPIDLE",
	"MEMCACHED_BEHAVIOR_LOAD_FROM_FILE",
	"MEMCACHED_BEHAVIOR_REMOVE_FAILED_SERVERS",
	"MEMCACHED_BEHAVIOR_DEAD_TIMEOUT",
	"MEMCACHED_BEHAVIOR_SERVER_TIMEOUT_LIMIT",
	"MEMCACHED_BEHAVIOR_MAX"
};
#define MEMCACHED_BEHAVIORS_COUNT (ARRAY_SIZE(MEMCACHED_BEHAVIOR_NAMES))

const char* MEMCACHED_HASH_NAMES[] = {
	"MEMCACHED_HASH_DEFAULT",
	"MEMCACHED_HASH_MD5",
	"MEMCACHED_HASH_CRC",
	"MEMCACHED_HASH_FNV1_64",
	"MEMCACHED_HASH_FNV1A_64",
	"MEMCACHED_HASH_FNV1_32",
	"MEMCACHED_HASH_FNV1A_32",
	"MEMCACHED_HASH_HSIEH",
	"MEMCACHED_HASH_MURMUR",
	"MEMCACHED_HASH_JENKINS",
	"MEMCACHED_HASH_MURMUR3",
	"MEMCACHED_HASH_CUSTOM",
	"MEMCACHED_HASH_MAX",
};
#define MEMCACHED_HASH_COUNT (ARRAY_SIZE(MEMCACHED_HASH_NAMES))

const char* MEMCACHED_DISTRIBUTION_NAMES[] = {
	"MEMCACHED_DISTRIBUTION_MODULA",
	"MEMCACHED_DISTRIBUTION_CONSISTENT",
	"MEMCACHED_DISTRIBUTION_CONSISTENT_KETAMA",
	"MEMCACHED_DISTRIBUTION_RANDOM",
	"MEMCACHED_DISTRIBUTION_CONSISTENT_KETAMA_SPY",
	"MEMCACHED_DISTRIBUTION_CONSISTENT_WEIGHTED",
	"MEMCACHED_DISTRIBUTION_VIRTUAL_BUCKET",
	"MEMCACHED_DISTRIBUTION_CONSISTENT_MAX"
};
#define MEMCACHED_DISTRIBUTION_COUNT (ARRAY_SIZE(MEMCACHED_DISTRIBUTION_NAMES))

VALUE
rb_connection_get_behavior(VALUE self, VALUE rb_behavior)
{
	memcached_st *mc;
	memcached_behavior_t behavior;
	uint64_t ret;

	UnwrapMemcached(self, mc);
	Check_Type(rb_behavior, T_FIXNUM);

	behavior = FIX2INT(rb_behavior);
	ret = memcached_behavior_get(mc, behavior);

	switch (behavior) {
	case MEMCACHED_BEHAVIOR_HASH:
	case MEMCACHED_BEHAVIOR_DISTRIBUTION:
		return ULL2NUM(ret);
	default:
		return ret ? Qtrue : Qfalse;
	}
}

VALUE
rb_connection_set_behavior(VALUE self, VALUE rb_behavior, VALUE rb_value)
{
	memcached_st *mc;
	memcached_return_t rc;
	memcached_behavior_t behavior;
	uint64_t value = 0;

	UnwrapMemcached(self, mc);
	Check_Type(rb_behavior, T_FIXNUM);

	behavior = FIX2INT(rb_behavior);

	switch (TYPE(rb_value)) {
	case T_TRUE:
		value = 1;
		break;
	case T_FIXNUM:
		value = FIX2INT(rb_value);
		break;
	case T_BIGNUM:
	case T_FLOAT:
		value = NUM2INT(rb_value);
		break;
	case T_NIL:
	case T_FALSE:
		break;
	default:
		rb_raise(rb_eTypeError, "unexpected type for 'behavior'");
		break;
	}

	rc = memcached_behavior_set(mc, behavior, value);
	rb_memcached_error_check(rc);

	return (rc == MEMCACHED_SUCCESS) ? Qtrue : Qfalse;
}

void
Init_memcached_rb_behavior(void)
{
	size_t i;
	VALUE rb_mBehaviors = rb_define_module_under(rb_mMemcached, "Behaviors");

	for (i = 0; i < MEMCACHED_BEHAVIORS_COUNT; i++) {
		rb_define_const(rb_mBehaviors, MEMCACHED_BEHAVIOR_NAMES[i], INT2FIX(i));
	}
	for (i = 0; i < MEMCACHED_HASH_COUNT; i++) {
		rb_define_const(rb_mBehaviors, MEMCACHED_HASH_NAMES[i], INT2FIX(i));
	}
	for (i = 0; i < MEMCACHED_DISTRIBUTION_COUNT; i++) {
		rb_define_const(rb_mBehaviors, MEMCACHED_DISTRIBUTION_NAMES[i], INT2FIX(i));
	}
}
