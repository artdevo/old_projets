from scapy.all import *
import sys
import socket
import time

def get_hostname(ip):
    """ Получает словесное имя хоста по IP-адресу, если есть"""
    try:
        # Обратный DNS запрос (как NSlookup)
        hostname = socket.gethostbyaddr(ip)[0]
        return hostname
    except socket.herror:
        return None

def traceroute(dest_ip, max_hops=30, timeout=2):
    """ Аналог утилиты Traceroute. Считает пройденные хосты 
    до dest_ip, по дефолту не более 30 хостов, с ожидание 2 с
    """
    ttl = 1
    print("Hop\tAddress\t\t\tHostname\t\tRTT(ms)")  # Заголовки
    while True:
        try:
            # Создание пакета IP и UDP
            pkt = IP(dst=dest_ip, ttl=ttl) / UDP(dport=33434)
            start_time = time.time()
            # Отправка пакета
            reply = sr1(pkt, verbose=0, timeout=timeout)
            end_time = time.time()
            if reply:
                rtt_ms = (end_time - start_time) * 1000
            else:
                # Нет ответа
                rtt_ms = 0
        
            if reply is None:
                print(f"{ttl}\t*\t\t\t*\t\t\t*")
            else:
                # Получен ответ
                hostname = get_hostname(reply.src)
                hostname = hostname if hostname else "*"
                print(f"{ttl}\t{reply.src:<20}\t{hostname:24}{rtt_ms:.2f}")
                if reply.type == 3:
                    print(f"Destination {dest_ip} reached")
                    break
        
            ttl += 1
            if ttl > max_hops:
                # Превышение количества прыжков
                print("Number of hops is too big")
                break

        except Exception as e:
            print(f"{ttl}\tError: {e}")
            ttl += 1
            if ttl > max_hops:
              print("Number of hops is too big")
              break

def is_valid_ip(ip):
    try:
        socket.inet_aton(ip)
        return True
    except socket.error:
        return False

def is_valid_hostname(hostname):
    try:
        # Попытка разрешить имя хоста в IP-адрес
        socket.gethostbyname(hostname)
        return True
    except socket.error:
        return False

def main():
    # Проверка количества аргументов
    if len(sys.argv) != 2:
        print('Wrong number of comand line args')
        sys.exit(1)

    # Получение ip-адреса из аргумента командной строки
    dest_ip = sys.argv[1]
    if is_valid_ip(dest_ip):
        print(f"Tracing to {dest_ip}, max_hops = 30, timeout = 2")
        traceroute(dest_ip)
    elif is_valid_hostname(dest_ip):
        print(f"Tracing to {dest_ip}, max_hops = 30, timeout = 2")
        traceroute(dest_ip)
    else:
        print(f"Error: Invalid IP address or hostname: {dest_ip}")
        sys.exit(1)


if __name__ == "__main__":
    main()
