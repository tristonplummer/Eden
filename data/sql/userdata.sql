--
-- PostgreSQL database dump
--

-- Dumped from database version 12.4 (Ubuntu 12.4-0ubuntu0.20.04.1)
-- Dumped by pg_dump version 12.4

-- Started on 2020-10-03 02:20:11

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 6 (class 2615 OID 24578)
-- Name: userdata; Type: SCHEMA; Schema: -; Owner: -
--

CREATE SCHEMA userdata;


--
-- TOC entry 207 (class 1255 OID 24596)
-- Name: login(character varying, character varying, character varying); Type: FUNCTION; Schema: userdata; Owner: -
--

CREATE FUNCTION userdata.login(name character varying, password character varying, "ipAddress" character varying, OUT userid integer, OUT status smallint, OUT privilege smallint) RETURNS record
    LANGUAGE plpgsql
    AS $$
BEGIN
	userId = 1;
	status = 0;
	privilege = 0;
END
$$;


SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 206 (class 1259 OID 24581)
-- Name: users; Type: TABLE; Schema: userdata; Owner: -
--

CREATE TABLE userdata.users (
    id integer NOT NULL,
    name character varying(19) NOT NULL,
    password character varying(32) NOT NULL,
    privilege smallint DEFAULT 0 NOT NULL
);


--
-- TOC entry 205 (class 1259 OID 24579)
-- Name: users_id_seq; Type: SEQUENCE; Schema: userdata; Owner: -
--

ALTER TABLE userdata.users ALTER COLUMN id ADD GENERATED ALWAYS AS IDENTITY (
    SEQUENCE NAME userdata.users_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- TOC entry 2932 (class 0 OID 24581)
-- Dependencies: 206
-- Data for Name: users; Type: TABLE DATA; Schema: userdata; Owner: -
--

COPY userdata.users (id, name, password, privilege) FROM stdin;
\.


--
-- TOC entry 2938 (class 0 OID 0)
-- Dependencies: 205
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: userdata; Owner: -
--

SELECT pg_catalog.setval('userdata.users_id_seq', 1, false);


--
-- TOC entry 2804 (class 2606 OID 24586)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: userdata; Owner: -
--

ALTER TABLE ONLY userdata.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


-- Completed on 2020-10-03 02:20:11

--
-- PostgreSQL database dump complete
--

