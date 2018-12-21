-- phpMyAdmin SQL Dump
-- version 4.6.6deb4
-- https://www.phpmyadmin.net/
--
-- Хост: localhost:3306
-- Время создания: Дек 16 2018 г., 20:05
-- Версия сервера: 10.1.26-MariaDB-0+deb9u1
-- Версия PHP: 7.0.30-0+deb9u1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- База данных: `allcash`
--

-- --------------------------------------------------------

--
-- Структура таблицы `allcash_admins`
--

CREATE TABLE `allcash_admins` (
  `id` int(11) NOT NULL,
  `login` varchar(32) NOT NULL,
  `password` varchar(32) NOT NULL,
  `first_name` varchar(40) NOT NULL,
  `second_name` varchar(40) NOT NULL,
  `middle_name` varchar(40) NOT NULL,
  `can_add_admin` tinyint(1) NOT NULL,
  `can_edit_phone` tinyint(1) NOT NULL,
  `can_edit_password` tinyint(1) NOT NULL,
  `can_edit_personal_data` tinyint(1) NOT NULL,
  `can_edit_balance` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Структура таблицы `allcash_system`
--

CREATE TABLE `allcash_system` (
  `id` int(11) NOT NULL,
  `key` varchar(40) NOT NULL,
  `value` varchar(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `allcash_system`
--

INSERT INTO `allcash_system` (`id`, `key`, `value`) VALUES
(1, 'last_wallet_id', '0000000000'),
(2, 'last_version', '1.0');

-- --------------------------------------------------------

--
-- Структура таблицы `allcash_users`
--

CREATE TABLE `allcash_users` (
  `id` int(11) NOT NULL,
  `phone` varchar(13) NOT NULL,
  `password` varchar(32) NOT NULL,
  `first_name` varchar(40) NOT NULL,
  `second_name` varchar(40) NOT NULL,
  `middle_name` varchar(40) NOT NULL,
  `reg_date` bigint(20) NOT NULL,
  `balance` decimal(10,2) NOT NULL,
  `wallet_id` varchar(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `allcash_admins`
--
ALTER TABLE `allcash_admins`
  ADD PRIMARY KEY (`id`);

--
-- Индексы таблицы `allcash_system`
--
ALTER TABLE `allcash_system`
  ADD PRIMARY KEY (`id`);

--
-- Индексы таблицы `allcash_users`
--
ALTER TABLE `allcash_users`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT для сохранённых таблиц
--

--
-- AUTO_INCREMENT для таблицы `allcash_system`
--
ALTER TABLE `allcash_system`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
