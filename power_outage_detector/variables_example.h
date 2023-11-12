/* Copyright (C) 2023 Renan Alencar - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the GNU license, which unfortunately won't be
 * written for another century.
 *
 * You should have received a copy of the GNU license with
 * this file. If not, please write to: renan.costaalencar@gmail.com,
 * or visit : https://spdx.org/licenses/GPL-3.0-or-later.html
 *
 * Project based on DETETOR DE FALHA DE ENERGIA published by 
 * Electrofun Blog (https://www.electrofun.pt/blog/detetor-de-falha-de-energia-com-esp32/)
 */

/**
 * @file variables.h
 * @brief Header responsible for storing sensitive data.
 * @author Renan Alencar
 * Contact: https://linktr.ee/mr.costaalencar
 *
 */

const static char* MESSAGE = "<ADD HERE THE MESSAGE YOU WANT TO SEND IN THE SMS>";
const static char* NUMBERS[] = {"<RECIPIENT_1>", "<RECIPIENT_2>"}; // Add phone numbers with "+" and the contry code, e.g. +17077225041