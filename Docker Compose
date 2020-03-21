# Docker Home Assistant QNAP

####  QNAP: docker-compose: HomeAssistant + Mosquitto + Caddy + Configurator + FtpServer + Doods

version: '3'
services:
  mosquitto:
    container_name: mosquitto
    image: eclipse-mosquitto:latest
    restart: always
    ports:
      - 1883:1883
    volumes:
      - /share/Public/container-station-data/mosquitto/config:/mosquitto/config
      - /share/Public/container-station-data/mosquitto/data:/mosquitto/data
      - /share/Public/container-station-data/mosquitto/log:/mosquitto/log
    network_mode:
      "host"
  homeassistant:
    container_name: home-assistant
    image: homeassistant/home-assistant:latest
    restart: always
    ports:
      - 8123:8123
    volumes:
      - /share/Public/container-station-data/homeassistant:/config
      - /etc/localtime:/etc/localtime:ro
    depends_on:
      - mosquitto
    network_mode:
      "host"
  hass-configurator:
    image: "causticlab/hass-configurator-docker:x86_64"
    container_name: "hass-configurator"
    restart: always
    ports:
      - "3218:3218/tcp"
    network_mode:
      "host"
    volumes:
      - /share/Public/container-station-data/homeassistant:/config
  ftpserver:
    container_name: ftpserver
    image: bogem/ftp:latest
    restart: always
    environment:
      - FTP_USER=mttstt
      - FTP_PASS=mttstt
      - PASV_ADDRESS=192.168.1.108
    volumes:
      - /share/Public/container-station-data/homeassistant/www:/home/vsftpd
    ports:
      - "20:20/tcp"
      - "21:21/tcp"
      - "47400-47470:47400-47470"
    network_mode:
      "host"
  doods:
    container_name: doods
    image: snowzach/doods:noavx
    volumes:
      - /share/Public/container-station-data/doods/models:/opt/doods/models
      - /share/Public/container-station-data/doods/config.yaml:/opt/doods/config.yaml
    ports:
      - "8080:8080/tcp"
    network_mode:
      "host"
  caddy:
    container_name: caddy
    image: abiosoft/caddy:latest
    restart: always
    environment:
      - ACME_AGREE=true
      - ENABLE_TELEMETRY=false
      - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
    volumes:
      - /share/Public/container-station-data/caddy/etc:/etc/caddy
      - /share/Public/container-station-data/caddy/srv:/srv
      - /share/Public/container-station-data/caddy/.root:/root/.caddy
    command: ["--conf", "/etc/caddy/Caddyfile", "--log", "stdout", "--agree=true"]
    network_mode:
      "host"
