# -*- coding: utf-8 -*-

import sys
import time
import telepot
from telepot.loop import MessageLoop
import numpy as np
import pandas as pd
import paho.mqtt.client as mqtt
from telepot.namedtuple import ReplyKeyboardMarkup, KeyboardButton


std = """
Oi, sou um bot para Controle de maquete {^_^}
\nLista de comandos base
/garagem abrir ou fechar\n
/portao on ou off\n
/banheiro on ou off\n
/sala on ou off\n
/quarto on ou off\n

 """

client = mqtt.Client()
# conecta no broker
client.connect("10.90.5.146", 1883)


def Control(msg,chat_id):
   if '/garagem' in msg:
      if "abrir" in msg:
          client.publish("/makete/","l")
          bot.sendMessage(chat_id, "Garagem Aperta !")
      if "fechar" in msg:
          client.publish("/makete/","k")    
          bot.sendMessage(chat_id, "Garagem Fechada !")

   if '/quarto' in msg:
      if "on" in msg:
          client.publish("/makete/","q")
          bot.sendMessage(chat_id, "Luz do quarto Ligada !")
      if "off" in msg:
          client.publish("/makete/","w")    
          bot.sendMessage(chat_id, "Luz do quarto Desligada !")

   if '/sala' in msg:
      if "on" in msg:
          client.publish("/makete/","e")
          bot.sendMessage(chat_id, "Luz da sala Ligada !")

      if "off" in msg:
          client.publish("/makete/","r")    
          bot.sendMessage(chat_id, "Luz da sala Desligada !")

   if '/suite' in msg:
      if "on" in msg:
          client.publish("/makete/","t")
          bot.sendMessage(chat_id, "Luz da suite Ligada !")

      if "off" in msg:
          client.publish("/makete/","y")    
          bot.sendMessage(chat_id, "Luz da suite Desligada !")

   if '/banheiro' in msg:
      if "on" in msg:
          client.publish("/makete/","u")
          bot.sendMessage(chat_id, "Luz do banheiro Ligada !")

      if "off" in msg:
          client.publish("/makete/","i")    
          bot.sendMessage(chat_id, "Luz do banheiro Desligada !")

   if '/portao' in msg:
      if "on" in msg:
          client.publish("/makete/","o")
          bot.sendMessage(chat_id, "Luz do portão Ligada !")

      if "off" in msg:
          client.publish("/makete/","a")    
          bot.sendMessage(chat_id, "Luz do portão Desligada !")


def handle(msg):
    content_type, chat_type, chat_id = telepot.glance(msg)
    print(content_type, chat_type, chat_id)
    
    if content_type == 'text':
      print msg['text']
      Control(msg['text'],chat_id)
      #button(msg['text'],chat_id)
      if msg['text'] == "Oi":
          bot.sendMessage(chat_id, std)
    if content_type == 'text':
        if msg['text'] == 'sala':
            bot.sendMessage(chat_id, 'Controle',
                            reply_markup=ReplyKeyboardMarkup(
                                keyboard=[
                                    [KeyboardButton(text="/sala on"), 
                                     KeyboardButton(text="/sala off")]
                                ]
                            ))
        if msg['text'] == 'quarto':
            bot.sendMessage(chat_id, 'Controle',
                            reply_markup=ReplyKeyboardMarkup(
                                keyboard=[
                                    [KeyboardButton(text="/quarto on"), 
                                     KeyboardButton(text="/quarto off")]
                                ]
                            ))
        if msg['text'] == 'portao':
            bot.sendMessage(chat_id, 'Controle',
                            reply_markup=ReplyKeyboardMarkup(
                                keyboard=[
                                    [KeyboardButton(text="/portao on"), 
                                     KeyboardButton(text="/portao off")]
                                ]
                            ))
        if msg['text'] == 'banheiro':
            bot.sendMessage(chat_id, 'Controle',
                            reply_markup=ReplyKeyboardMarkup(
                                keyboard=[
                                    [KeyboardButton(text="/banheiro on"), 
                                     KeyboardButton(text="/banheiro off")]
                                ]
                            ))
        if msg['text'] == 'suite':
            bot.sendMessage(chat_id, 'Controle',
                            reply_markup=ReplyKeyboardMarkup(
                                keyboard=[
                                    [KeyboardButton(text="/suite on"), 
                                     KeyboardButton(text="/suite off")]
                                ]
                            ))
        if msg['text'] == 'garagem':
            bot.sendMessage(chat_id, 'Controle',
                            reply_markup=ReplyKeyboardMarkup(
                                keyboard=[
                                    [KeyboardButton(text="/garagem abrir"), 
                                     KeyboardButton(text="/garagem fechar")]
                                ]
                            ))

       


#TOKEN = sys.argv[1]  # get token from command-line

bot = telepot.Bot('721933323:AAHUZ_fEYIAX0zNN3rn3HMKj0kfmvTcZj5I')
MessageLoop(bot, handle).run_as_thread()
print ('Listening ...')

# Keep the program running.
while 1:
    time.sleep(10)
