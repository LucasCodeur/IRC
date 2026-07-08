/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NumericReplies.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enchevri <enchevri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 13:39:39 by kbarru            #+#    #+#             */
/*   Updated: 2026/07/03 16:52:08 by enchevri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICREPLIES_H
# define NUMERICREPLIES_H

# define RPL_WELCOME			"001"
# define RPL_YOURHOST			"002"
# define RPL_CREATED			"003"
# define RPL_MYINFO				"004"
# define RPL_ENDOFWHO			"315"
# define RPL_CHANNELMODEIS		"324"
# define RPL_NOTOPIC			"331"
# define RPL_TOPIC				"332"
# define RPL_WHOREPLY			"352"
# define RPL_NAMREPLY			"353"
# define RPL_ENDOFNAMES			"366"
# define ERR_NOSUCHNICK			"401"
# define ERR_NOSUCHCHANNEL		"403"
# define ERR_UNKNOWNCOMMAND		"421"
# define ERR_NONICKNAMEGIVEN	"431"
# define ERR_ERRONEUSNICKNAME	"432"
# define ERR_NICKNAMEINUSE		"433"
# define ERR_NICKCOLLISION		"436"
# define RPL_INVITING			"341"
# define ERR_NOTONCHANNEL		"442"
# define ERR_USERONCHANNEL		"443"
# define ERR_NOTREGISTERED		"451"
# define ERR_NEEDMOREPARAMS		"461"
# define ERR_ALREADYREGISTRED	"462"
# define ERR_CHANNELISFULL		"471"
# define ERR_UNKNOWNMODE		"472"
# define ERR_INVITEONLYCHAN		"473"
# define ERR_BADCHANNELKEY		"475"
# define ERR_CHANOPRIVSNEEDED	"482"

# define LF "\n"
# define CR "\r"

#endif
