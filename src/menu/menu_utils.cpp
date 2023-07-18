#include "menu_utils.h"

static HardwareSerial *_stream = &Serial;
static bool echo = true;

void setMenuStream(HardwareSerial *stream)
{
    _stream = stream;
}

int printfln(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int space = vsnprintf(NULL, 0, format, args) + 1;
    char buf[space];
    memset(buf, 0, space);
    vsnprintf(buf, space, format, args);
    va_end(args);

    int size = strlen(buf);
    _stream->write((uint8_t*)buf, size);
    _stream->println();
    return size;
}

int printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int space = vsnprintf(NULL, 0, format, args) + 1;
    char buf[space];
    memset(buf, 0, space);
    vsnprintf(buf, space, format, args);
    va_end(args);

    int size = strlen(buf);
    _stream->write((uint8_t*)buf, size);
    return size;

}

size_t read_line(HardwareSerial &stream, char *val, size_t length)
{
    int current = 0;
    size_t count = 0;
    memset(val, 0, length);
    while(current != '\r' && current != '\n' && count < length-1)
    {

        yield();
        if(stream.available())
        {
            current = stream.read();
            if(current == 127)
            {
                if(count > 0)
                {
                    val[--count] = '\0';
                    if(echo){stream.print("\b \b");}
                }
            }
            else if(current == 27)
            {
                break;
            }
            else if(current != '\r' && current != '\n')
            {
                val[count++] = (char)current;
                if(echo)
                {
                    stream.print((char)current);
                }
            }

        }
    }

    //If we have terminated on a \r and \r\n was sent there is an extra \n sitting in the buffer.
    //Clean up this extra \n.
    Serial.println();
    while(current == '\r' || current == '\n')
    {
        if(stream.available()) {
            if(stream.peek() == '\n' || stream.peek() == '\r')
            {
                current = stream.read();
            }
        }
        else
        {
            break;
        }
    }

    val[count] = 0;
    return count;
}

bool read_double_t(HardwareSerial &stream, double *val)
{
    char buf[31];
    size_t read_count = read_line(stream, buf, sizeof(buf)-1);
    if(read_count > 0)
    {
        buf[30] = 0;
        *val = (double)String(buf).toDouble();
    }
    return read_count > 0;
}

bool read_uint16_t(HardwareSerial &stream, uint16_t *val)
{
    char buf[31];
    size_t read_count = read_line(stream, buf, sizeof(buf)-1);
    if(read_count > 0)
    {
        buf[30] = 0;
        *val = (uint16_t)String(buf).toInt();
    }
    return read_count > 0;
}


bool read_int32_t(HardwareSerial &stream, int32_t *val)
{
    char buf[31];
    size_t read_count = read_line(stream, buf, sizeof(buf)-1);
    if(read_count > 0)
    {
        buf[30] = 0;
        *val = (int32_t)String(buf).toInt();
    }
    return read_count > 0;
}

bool read_int(HardwareSerial &stream, int *val)
{
    char buf[31];
    size_t read_count = read_line(stream, buf, sizeof(buf)-1);
    if(read_count > 0)
    {
        buf[30] = 0;
        *val = (int)String(buf).toInt();
    }
    return read_count > 0;
}

bool read_bool_t(HardwareSerial &stream, bool *val, bool *valid)
{
	*valid = false;
	char buf[31];
	memset(buf, 0, sizeof(buf));
	char * _true = "true";
	if(read_line(stream, buf, sizeof(buf)-1))
	{
		if(strnlen(buf, sizeof(buf)) > 1)
		{
			*val = strncasecmp(buf, _true, strlen(_true)) == 0;
			*valid = true;
			return true;
		}
		switch(buf[0])
		{
			case 'T':
			case 't':
			case 'y':
			case 'Y':
			case '1':
                *val = true;
                *valid = true;
                return true;
                break;
			case 'f':
			case 'F':
			case 'n':
			case 'N':
			case '0':
                *val = false;
                *valid = true;
                return true;
	    		break;
			default:
    			return true;
			
		}
	}
	return false;
}

void clearBuf()
{
    while(Serial.available() > 0)
    {
        Serial.read();
    }
}