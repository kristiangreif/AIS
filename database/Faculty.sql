-- phpMyAdmin SQL Dump
-- version 5.2.1-2.fc39
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Jan 18, 2024 at 07:50 PM
-- Server version: 10.5.23-MariaDB
-- PHP Version: 8.2.14

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `Faculty`
--

-- --------------------------------------------------------

--
-- Table structure for table `Courses`
--

CREATE TABLE `Courses` (
  `ID` int(11) NOT NULL,
  `name` varchar(10) NOT NULL,
  `teacher_first_name` varchar(20) NOT NULL,
  `teacher_last_name` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Dumping data for table `Courses`
--

INSERT INTO `Courses` (`ID`, `name`, `teacher_first_name`, `teacher_last_name`) VALUES
(1, 'B-PMEAP', 'Pavol', 'Bisták'),
(2, 'B-ELSA', 'Gabriel', 'Gálik'),
(3, 'B-PLCM', 'Ján', 'Cigánek'),
(4, 'B-PROPA', 'Dávid', 'Mikle');

-- --------------------------------------------------------

--
-- Table structure for table `Evaluation`
--

CREATE TABLE `Evaluation` (
  `ID` int(11) NOT NULL,
  `student_id` int(11) NOT NULL,
  `course_id` int(11) NOT NULL,
  `grade` varchar(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Dumping data for table `Evaluation`
--

INSERT INTO `Evaluation` (`ID`, `student_id`, `course_id`, `grade`) VALUES
(1, 1, 2, 'A'),
(8, 1, 3, 'B'),
(10, 11, 2, 'F'),
(11, 17, 1, 'C'),
(12, 19, 2, 'D'),
(16, 20, 1, 'C'),
(17, 23, 4, 'B'),
(18, 20, 2, 'C'),
(19, 20, 3, 'B'),
(20, 19, 3, 'A'),
(21, 22, 3, 'D'),
(22, 22, 4, 'E'),
(23, 30, 1, 'D'),
(24, 25, 4, 'B'),
(25, 23, 3, 'E'),
(26, 30, 2, 'A'),
(27, 32, 1, 'C'),
(28, 32, 3, 'A');

-- --------------------------------------------------------

--
-- Table structure for table `Students`
--

CREATE TABLE `Students` (
  `ID` int(11) NOT NULL,
  `first_name` varchar(20) NOT NULL,
  `last_name` varchar(30) NOT NULL,
  `name` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Dumping data for table `Students`
--

INSERT INTO `Students` (`ID`, `first_name`, `last_name`, `name`) VALUES
(1, 'Ján', 'Mrkva', 'Ján Mrkva'),
(11, 'Jana', 'Pazitkova', 'Jana Pazitkova'),
(17, 'Emil', 'Malý', 'Emil Malý'),
(19, 'Kamil', 'Obrovsky', 'Kamil Obrovsky'),
(20, 'Peter', 'Úzky', 'Peter Úzky'),
(21, 'Milan', 'Široký', 'Milan Široký'),
(22, 'Alojz', 'Krivý', 'Alojz Krivý'),
(23, 'Gertrúda', 'Tibenská', 'Gertrúda Tibenská'),
(25, 'Alfonz', 'Veľký', 'Alfonz Veľký'),
(30, 'Johanna', 'Záletná', 'Johanna Záletná'),
(32, 'Zlatica', 'Bystrá', 'Zlatica Bystrá');

--
-- Triggers `Students`
--
DELIMITER $$
CREATE TRIGGER `setName` BEFORE INSERT ON `Students` FOR EACH ROW SET NEW.name := CONCAT(NEW.first_name, CONCAT(" ", NEW.last_name))
$$
DELIMITER ;
DELIMITER $$
CREATE TRIGGER `setNameU` BEFORE UPDATE ON `Students` FOR EACH ROW SET NEW.name := CONCAT(NEW.first_name, CONCAT(" ", NEW.last_name))
$$
DELIMITER ;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `Courses`
--
ALTER TABLE `Courses`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `Evaluation`
--
ALTER TABLE `Evaluation`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `student_id` (`student_id`),
  ADD KEY `course_id` (`course_id`);

--
-- Indexes for table `Students`
--
ALTER TABLE `Students`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `Courses`
--
ALTER TABLE `Courses`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT for table `Evaluation`
--
ALTER TABLE `Evaluation`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=29;

--
-- AUTO_INCREMENT for table `Students`
--
ALTER TABLE `Students`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=33;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `Evaluation`
--
ALTER TABLE `Evaluation`
  ADD CONSTRAINT `Evaluation_ibfk_1` FOREIGN KEY (`student_id`) REFERENCES `Students` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `Evaluation_ibfk_2` FOREIGN KEY (`course_id`) REFERENCES `Courses` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
