/*
Keep a log of any SQL queries you execute as you solve the mystery.

Goal is to identify:
- Who the thief is,
- What city the thief escaped to, and
- Who the thief’s accomplice is who helped them escape

All you know is that the theft took place on July 28, 2021 and that it took place on Humphrey Street.
*/
-- Start by checking out the crime scene report for the incident
-- took place at 10:15 am
-- took place at the Humphrey Street baker
-- 3 witnesses - transcripts mention the baker
SELECT
  description
FROM
  crime_scene_reports
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND street = 'Humphrey Street'
  AND description LIKE 'Theft%';

-- Next let's find the interview transcripts
-- Ruth: within 10 min of theft, thief drives away, look at security footage
-- Eugene: recognized the person. saw them withdraw money from ATM on Leggett Street earlier that morning
-- Raymond: made a call while leaving. planing to take earliest flight on 7/29 and bought ticket
SELECT
  *
FROM
  interviews
WHERE
  transcript LIKE '%akery%'
  AND MONTH = 7;

-- Check phone logs - maybe this can help narrow down my time window
-- just has numbers and duration, no timestamp
-- LESS than 1 minute!
-- phone number subquery
SELECT
  *
FROM
  phone_calls
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND duration < 60;

-- Check bakery security footage from ~ 10:15 am - 10:20 am
-- 9 vehicles exited from 10:16 to 10:35 am
-- let's get the time of entrance as well
-- license plate subquery
SELECT
  *
FROM
  bakery_security_logs
WHERE
  license_plate IN (
    SELECT
      license_plate
    FROM
      bakery_security_logs
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 28
      AND HOUR = 10
      AND activity = 'exit'
  );

-- Check ATM footage from 7/28/21 before 10:15 am
-- 8 people match critera
-- can join to bank_accounts to get person id
-- suspicious accounts subquery
SELECT
  account_number
FROM
  atm_transactions
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND atm_location = 'Leggett Street'
  AND transaction_type = 'withdraw';

-- Check ticket reciepts from around 10:15 am for first flight out of town
-- First flight is to NYC
SELECT
  *
FROM
  airports
WHERE
  id = (
    SELECT
      destination_airport_id
    FROM
      flights
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 29
    ORDER BY
      HOUR,
      MINUTE
    LIMIT
      1
  );

-- Also get list of passport numbers from that flight
-- flight suspects
SELECT
  *
FROM
  passengers
WHERE
  flight_id = (
    SELECT
      id
    FROM
      flights
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 29
    ORDER BY
      HOUR,
      MINUTE
    LIMIT
      1
  );

-------------
-- APPROACH
------------
-- people table has phone number, passport number, licesnse plate number
-- gather up info from each query above and see if it matches that person at all
-- Once I know who the thief is, I can use the phone logs to find their accomplice
-- narrows it down to 2 people
SELECT
  *
FROM
  people
WHERE
  id IN (
    SELECT
      person_id
    FROM
      bank_accounts
    WHERE
      account_number IN (
        SELECT
          account_number
        FROM
          atm_transactions
        WHERE
          YEAR = 2021
          AND MONTH = 7
          AND DAY = 28
          AND atm_location = 'Leggett Street'
          AND transaction_type = 'withdraw'
      )
  )
  AND phone_number IN (
    SELECT
      caller
    FROM
      phone_calls
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 28
      AND duration < 60
  )
  AND license_plate IN (
    SELECT
      license_plate
    FROM
      bakery_security_logs
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 28
      AND HOUR = 10
      AND minute < 25
      AND activity = 'exit'
  )
  AND passport_number IN (
    SELECT
      passport_number
    FROM
      passengers
    WHERE
      flight_id = (
        SELECT
          id
        FROM
          flights
        WHERE
          YEAR = 2021
          AND MONTH = 7
          AND DAY = 29
        ORDER BY
          HOUR,
          MINUTE
        LIMIT
          1
      )
  );

-- currently have two suspects, how to narrow down?
/*
+--------+--------+----------------+-----------------+---------------+
|   id   |  name  |  phone_number  | passport_number | license_plate |
+--------+--------+----------------+-----------------+---------------+
| 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
| 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
+--------+--------+----------------+-----------------+---------------+

Let's go back through each line of evidence and see
*/
-- phone calls, both numbers have several calls on that day
SELECT
  *
FROM
  phone_calls
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND (
    caller = '(286) 555-6063'
    OR caller = '(367) 555-5533'
  );

-- bakery securtiy logs
-- 94KL13X left at 10:18 - is 2 min too fast for a robbery?
-- 1106N58 left at 10:35 - is 20 min too long to leave?
SELECT
  *
FROM
  bakery_security_logs
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND HOUR = 10
  AND activity = 'exit'
  AND (
    license_plate = '1106N58'
    OR license_plate = '94KL13X'
  );

-- Looking at seats, Taylor in in seat 6D next to 8294398571
-- whereas Bruce is sitting by himself in 4A
-- CHECK - if Taylor's call in the am is to a person with the
-- same passport number as the passenger in seat 6C then
-- that seems like it would confirm
-- NOPE not a match
-- could check other way? Is the receiver of the call also on the flight?
-- but no gauruntee that the accomplice is on the plane too

SELECT
  *
FROM
  people
WHERE passport_number = 8294398571;


-- After tweaking filter on time of the thief to leave the scene,
-- that would leave Bruce. so who is his accomplice?
-- let's see who he called during the crime
SELECT
  receiver
FROM
  phone_calls
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND duration < 60
  AND caller = '(367) 555-5533';

-- four different people! are any of them on the flight?
-- flight is number 36
-- NOPE
SELECT
*
FROM
people
WHERE phone_number IN (
    SELECT
  receiver
FROM
  phone_calls
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND duration < 60
  AND caller = '(367) 555-5533'
);