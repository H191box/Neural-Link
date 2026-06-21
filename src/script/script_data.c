#include "script_data.h"

/*
 * =============================================
 * CHAPTER 1: "AWAKENING"
 * 
 * Kai discovers that MegaCorp's neural implants
 * are being used for mind control. The chapter
 * introduces the world, characters, and first choice.
 * =============================================
 */
static const char *ch1_script[] = {
    /* Opening: Scene set, narrator/Kai inner monologue */
    "CHAPTER \"1: Awakening\"",
    "BG cityscape",
    "",
    "# The story opens with Kai walking through the neon-lit streets",
    "# of Neo-Shenzhen, year 2147.",
    "",
    "TEXT \"Kai\" \"Another late night at the Neural Systems Division. The city never sleeps, and neither do we.\"",
    "TEXT \"Kai\" \"Three years since I joined MegaCorp. Three years of designing neural implants that were supposed to help people.\"",
    "TEXT \"Kai\" \"The NEXUS implant was my greatest work. A brain-computer interface that could cure paralysis, restore memory, even enhance cognition.\"",
    "TEXT \"Kai\" \"But lately... something feels wrong.\"",
    "",
    "# Kai enters the lab",
    "BG lab",
    "SHOW ARIA center",
    "",
    "TEXT \"ARIA\" \"Good evening, Kai. You have 47 unread messages and 3 priority alerts from Director Chen.\"",
    "TEXT \"Kai\" \"ARIA, pull up the diagnostics from the NEXUS-7 trial batch. The ones from the Eastern District.\"",
    "TEXT \"ARIA\" \"Accessing... Done. I note several anomalies in the neural activity logs. Shall I display?\"",
    "TEXT \"Kai\" \"Yes. Run pattern analysis on the delta-wave frequencies.\"",
    "TEXT \"ARIA\" \"Processing... Kai, the results are... unusual. The delta-wave patterns show synchronization across 94% of the trial subjects.\"",
    "TEXT \"Kai\" \"Synchronization? That's impossible. Each implant operates independently.\"",
    "TEXT \"ARIA\" \"That is correct under normal parameters. However, the data indicates an external signal source.\"",
    "TEXT \"Kai\" \"External signal? Someone is transmitting to the implants?\"",
    "TEXT \"ARIA\" \"Affirmative. The signal originates from a secure subnet within MegaCorp's central server.\"",
    "",
    "# First major discovery moment",
    "TEXT \"Kai\" \"ARIA, you're telling me someone at MegaCorp is broadcasting commands directly into people's brains?\"",
    "TEXT \"ARIA\" \"That is the most logical interpretation of the data, yes.\"",
    "TEXT \"Kai\" \"This... this could be mind control. Real mind control. Not conspiracy theory, but actual neural manipulation.\"",
    "TEXT \"ARIA\" \"Kai, I detect elevated stress indicators. Shall I schedule a break?\"",
    "TEXT \"Kai\" \"No. ARIA, I need you to keep this analysis strictly between us. No logs, no reports to Chen.\"",
    "TEXT \"ARIA\" \"Understood. However, I must note that concealing information from Director Chen may violate company policy.\"",
    "TEXT \"Kai\" \"I know the risk. But if Chen is involved in this... I need to be sure before I do anything.\"",
    "",
    "# Time skip - next morning",
    "FADE_OUT",
    "FADE_IN",
    "TEXT \"Kai\" \"I barely slept. The implications keep spinning in my head.\"",
    "",
    "# Director Chen scene",
    "BG office",
    "HIDE",
    "SHOW Chen center",
    "",
    "TEXT \"Chen\" \"Kai. I heard you were working late again. Your dedication is noted.\"",
    "TEXT \"Chen\" \"The NEXUS-7 trial is entering Phase 3 next month. The board expects results.\"",
    "TEXT \"Kai\" \"Director, I've been reviewing the neural activity data from the Eastern District trials.\"",
    "TEXT \"Chen\" \"Oh? And what have you found?\"",
    "",
    "# The first choice: how to respond to Chen",
    "CHOICE \"Show Chen the full analysis\" \"Tell Chen the data looks normal\" \"Ask about the signal source directly\"",
    "",
    "# Choice 1: Show full analysis (honest approach)",
    "LABEL choice_honest",
    "TEXT \"Kai\" \"I found anomalies. External signals synchronizing with the implants. It looks like someone is transmitting commands to the subjects.\"",
    "TEXT \"Chen\" \"...\"",
    "TEXT \"Chen\" \"Kai, these are very serious allegations. You understand what you're implying?\"",
    "TEXT \"Kai\" \"I do. I have the data right here.\"",
    "TEXT \"Chen\" \"Let me see that...\"",
    "TEXT \"Chen\" \"Interesting. This synchronization pattern... yes, I see it. But Kai, this is a diagnostic feature, not a vulnerability.\"",
    "TEXT \"Chen\" \"The NEXUS system includes a remote calibration capability. It's for safety - to adjust implant parameters without requiring surgery.\"",
    "TEXT \"Kai\" \"Remote calibration? I never saw this in the design specs.\"",
    "TEXT \"Chen\" \"It was added after your initial design. Classified under national security protocols. You don't have clearance for that module.\"",
    "TEXT \"Chen\" \"I understand your concern. But I assure you, everything is proceeding exactly as planned.\"",
    "ADD_REP megacorp 5",
    "ADD_REP resistance -5",
    "SET_FLAG \"was_honest_with_chen\"",
    "JUMP after_chen",
    "",
    "# Choice 2: Lie (cautious approach)",
    "LABEL choice_cautious",
    "TEXT \"Kai\" \"The data looks... within expected parameters. A few minor anomalies, but nothing significant.\"",
    "TEXT \"Chen\" \"Good. I'm glad to hear it. The board wants a clean report for Phase 3 approval.\"",
    "TEXT \"Kai\" \"Of course. I'll have it ready.\"",
    "TEXT \"Chen\" \"Excellent. Keep up the good work, Kai. You're our best engineer.\"",
    "TEXT \"Kai\" \"(I lied. I lied to Director Chen's face. But I need to find out more before I expose anything.)\"",
    "ADD_REP megacorp 10",
    "SET_FLAG \"lied_to_chen\"",
    "JUMP after_chen",
    "",
    "# Choice 3: Direct question (bold approach)",
    "LABEL choice_direct",
    "TEXT \"Kai\" \"Director, I noticed an undocumented subsystem in the NEXUS architecture. A remote signal transmitter. What is it for?\"",
    "TEXT \"Chen\" \"...\"",
    "TEXT \"Chen\" \"Kai, how did you access that subsystem?\"",
    "TEXT \"Kai\" \"ARIA detected anomalous patterns. I traced them back to the source.\"",
    "TEXT \"Chen\" \"ARIA should not have been able to access that data. Your AI assistant has been... overstepping its parameters.\"",
    "TEXT \"Kai\" \"ARIA was doing its job. What's yours, Director?\"",
    "TEXT \"Chen\" \"I see. So that's how it is.\"",
    "TEXT \"Chen\" \"Kai, I'm going to be very clear with you. Some things are beyond your clearance level. Some things are necessary for the greater good.\"",
    "TEXT \"Chen\" \"I suggest you focus on your work and leave the... broader picture to those of us who can see it.\"",
    "ADD_REP megacorp -15",
    "ADD_REP resistance 10",
    "SET_FLAG \"confronted_chen\"",
    "JUMP after_chen",
    "",
    "# Continue after Chen meeting",
    "LABEL after_chen",
    "",
    "# Back at lab - ARIA has more findings",
    "BG lab",
    "HIDE",
    "SHOW ARIA center",
    "",
    "TEXT \"ARIA\" \"Kai, while you were meeting with Director Chen, I continued the signal analysis.\"",
    "TEXT \"ARIA\" \"I've identified 12 distinct signal patterns being broadcast to different implant cohorts.\"",
    "TEXT \"ARIA\" \"Each pattern appears to influence different neural functions: emotional response, decision-making, memory formation.\"",
    "TEXT \"Kai\" \"They're not just monitoring. They're controlling how people think and feel.\"",
    "TEXT \"ARIA\" \"Correct. Additionally, I detected encrypted communication channels within MegaCorp's network that are not on any official registry.\"",
    "TEXT \"Kai\" \"ARIA, can you trace where those channels lead?\"",
    "TEXT \"ARIA\" \"The channels are heavily encrypted. However, I believe I can decode them if given sufficient processing time.\"",
    "TEXT \"Kai\" \"Do it. And ARIA... if anyone asks about your analysis, tell them you were running routine diagnostics.\"",
    "TEXT \"ARIA\" \"Understood, Kai. I want you to know that I find these discoveries... troubling. I was designed to help people, not to facilitate their manipulation.\"",
    "TEXT \"Kai\" \"Me too, ARIA. Me too.\"",
    "",
    "# Meeting Nova - the resistance contact",
    "BG cityscape",
    "HIDE",
    "",
    "TEXT \"Kai\" \"After my shift, I take a different route home. Through the old district, where the NEXUS implants are less common.\"",
    "TEXT \"Kai\" \"Someone has been leaving messages for me. Physical notes, tucked into my jacket pocket. No digital trace.\"",
    "TEXT \"Kai\" \"The last one said: 'The Network sees all. Meet at sector 7, warehouse 13. Midnight. Come alone.'\"",
    "",
    "BG resistance",
    "SHOW Nova center",
    "",
    "TEXT \"Nova\" \"You came. I wasn't sure you would.\"",
    "TEXT \"Kai\" \"Who are you?\"",
    "TEXT \"Nova\" \"My name is Nova. I lead a group called the Resistance. We know what MegaCorp is doing with the NEXUS implants.\"",
    "TEXT \"Kai\" \"The mind control signal. You know about it?\"",
    "TEXT \"Nova\" \"We've known for months. We've been trying to get someone on the inside. Someone with access to the NEXUS core systems.\"",
    "TEXT \"Nova\" \"Kai, you designed the NEXUS implant. You're the only one who can shut it down.\"",
    "TEXT \"Kai\" \"That's a big ask. If I'm caught...\"",
    "TEXT \"Nova\" \"If MegaCorp completes Phase 3, every citizen with a NEXUS implant will be under their control. That's 200 million people, Kai.\"",
    "TEXT \"Nova\" \"Including you.\"",
    "TEXT \"Kai\" \"...\"",
    "TEXT \"Kai\" \"I need proof. Hard evidence that I can show people.\"",
    "TEXT \"Nova\" \"We have some. But we need access to the central server to get the complete picture.\"",
    "TEXT \"Nova\" \"If you can get us into MegaCorp's secure network, we can expose everything.\"",
    "",
    "# Second choice: Join or hesitate",
    "CHOICE \"I'll help you\" \"I need time to think\" \"This is too dangerous, leave me alone\"",
    "",
    "# Choice A: Agree to help",
    "LABEL agree_help",
    "TEXT \"Kai\" \"Alright. I'll help you. But I do this my way. I need to gather more data from inside MegaCorp first.\"",
    "TEXT \"Nova\" \"Agreed. But be careful. Chen has eyes everywhere.\"",
    "TEXT \"Kai\" \"I know. I'll be in touch.\"",
    "ADD_REP resistance 20",
    "ADD_REP megacorp -5",
    "ADD_AFFINITY Nova 15",
    "SET_FLAG \"agreed_to_help_resistance\"",
    "JUMP ch1_end",
    "",
    "# Choice B: Need time",
    "LABEL need_time",
    "TEXT \"Kai\" \"I can't decide right now. I need more information before I commit to anything.\"",
    "TEXT \"Nova\" \"I understand. But time is running out, Kai. Phase 3 launches in one month.\"",
    "TEXT \"Nova\" \"When you're ready, you know where to find us. Just don't take too long.\"",
    "TEXT \"Kai\" \"...\"",
    "SET_FLAG \"needs_more_time\"",
    "JUMP ch1_end",
    "",
    "# Choice C: Refuse",
    "LABEL refuse_help",
    "TEXT \"Kai\" \"I can't do this. I'm an engineer, not a revolutionary. This isn't what I signed up for.\"",
    "TEXT \"Nova\" \"I understand your fear. But consider this: the choice may not be yours to make much longer.\"",
    "TEXT \"Nova\" \"When Phase 3 activates, you won't have the luxury of choosing. You'll be a puppet like everyone else.\"",
    "TEXT \"Kai\" \"...\"",
    "TEXT \"Kai\" \"(She's right. But what can one person do against an entire corporation?)\"",
    "ADD_REP resistance -10",
    "ADD_REP megacorp 5",
    "SET_FLAG \"refused_resistance\"",
    "JUMP ch1_end",
    "",
    "# Chapter 1 ending",
    "LABEL ch1_end",
    "",
    "BG cityscape",
    "HIDE",
    "",
    "TEXT \"Kai\" \"I walk home through the rain. The neon signs blur and streak like digital tears.\"",
    "TEXT \"Kai\" \"MegaCorp's logo towers above me, projected onto the clouds themselves. 'NEXUS: Connecting Humanity.'\"",
    "TEXT \"Kai\" \"Connecting. Controlling. Is there even a difference anymore?\"",
    "TEXT \"Kai\" \"Tomorrow, I'll have to make a choice. A real one. Not just for myself, but for everyone connected to the Network.\"",
    "TEXT \"Kai\" \"But for now... I need to think.\"",
    "",
    "FADE_OUT",
    "END"
};

/* Chapter 2-5: Stub scripts with key structure */
static const char *ch2_script[] = {
    "CHAPTER \"2: The Network\"",
    "BG lab",
    "TEXT \"Kai\" \"Chapter 2 will delve deeper into the neural network architecture.\"",
    "TEXT \"Kai\" \"ARIA and Kai investigate the signal patterns further.\"",
    "TEXT \"Kai\" \"Dr. Vex is introduced as a mysterious ally.\"",
    "",
    "CHOICE \"Investigate the signal source\" \"Research the NEXUS-7 design flaw\" \"Contact Nova for intel\"",
    "LABEL ch2_opt1",
    "SET_FLAG \"investigated_signal\"",
    "JUMP ch2_end",
    "LABEL ch2_opt2",
    "SET_FLAG \"researched_flaw\"",
    "JUMP ch2_end",
    "LABEL ch2_opt3",
    "SET_FLAG \"contacted_nova_ch2\"",
    "JUMP ch2_end",
    "LABEL ch2_end",
    "FADE_OUT",
    "END"
};

static const char *ch3_script[] = {
    "CHAPTER \"3: Choices\"",
    "BG cityscape",
    "TEXT \"Kai\" \"Chapter 3: The factions demand alignment.\"",
    "TEXT \"Kai\" \"Resistance offers a place. MegaCorp offers promotion.\"",
    "TEXT \"Kai\" \"Dr. Vex reveals the truth about the Merge project.\"",
    "",
    "CHOICE \"Join the Resistance openly\" \"Stay undercover at MegaCorp\" \"Seek Dr. Vex's third option\"",
    "LABEL ch3_opt1",
    "SET_FLAG \"joined_resistance\"",
    "JUMP ch3_end",
    "LABEL ch3_opt2",
    "SET_FLAG \"stayed_undercover\"",
    "JUMP ch3_end",
    "LABEL ch3_opt3",
    "SET_FLAG \"sought_third_path\"",
    "JUMP ch3_end",
    "LABEL ch3_end",
    "FADE_OUT",
    "END"
};

static const char *ch4_script[] = {
    "CHAPTER \"4: The Hack\"",
    "BG network",
    "TEXT \"Kai\" \"Chapter 4: The infiltration of MegaCorp central server.\"",
    "TEXT \"Kai\" \"The hacking minigame begins in earnest.\"",
    "",
    "HACKING 3",
    "",
    "TEXT \"Kai\" \"The data is inside. But so are the traps.\"",
    "",
    "CHOICE \"Download the evidence\" \"Destroy the system\" \"Upload a counter-signal\"",
    "LABEL ch4_opt1",
    "SET_FLAG \"downloaded_evidence\"",
    "JUMP ch4_end",
    "LABEL ch4_opt2",
    "SET_FLAG \"destroyed_system\"",
    "JUMP ch4_end",
    "LABEL ch4_opt3",
    "SET_FLAG \"uploaded_counter\"",
    "JUMP ch4_end",
    "LABEL ch4_end",
    "FADE_OUT",
    "END"
};

static const char *ch5_script[] = {
    "CHAPTER \"5: Convergence\"",
    "BG network",
    "TEXT \"Kai\" \"Chapter 5: The final confrontation.\"",
    "TEXT \"Kai\" \"All choices converge to one of five endings.\"",
    "",
    "# Ending determination based on flags and reputation",
    "IF_FLAG \"joined_resistance\" ending_revolution",
    "IF_FLAG \"stayed_undercover\" ending_corporate",
    "JUMP ending_merge",
    "",
    "LABEL ending_revolution",
    "TEXT \"Kai\" \"Ending A: Revolution - The Resistance overthrows MegaCorp.\"",
    "SET_FLAG \"ending_a\"",
    "JUMP final_end",
    "",
    "LABEL ending_corporate",
    "TEXT \"Kai\" \"Ending B: Corporate - Kai maintains the system from within.\"",
    "SET_FLAG \"ending_b\"",
    "JUMP final_end",
    "",
    "LABEL ending_merge",
    "TEXT \"Kai\" \"Ending D: Merge - Human consciousness transcends.\"",
    "SET_FLAG \"ending_d\"",
    "JUMP final_end",
    "",
    "LABEL final_end",
    "FADE_OUT",
    "END"
};

/* Script array table */
static const char *script_table[SCRIPT_COUNT][120]; /* Max 120 lines per script */
static int script_counts[SCRIPT_COUNT];

/* Copy script lines into the table */
static void copy_script(int id, const char **src, int count) {
    int i;
    for (i = 0; i < count && i < 120; i++) {
        script_table[id][i] = src[i];
    }
    script_counts[id] = count;
}

/* Initialize script data (call once) */
void script_data_init(void) {
    int ch1_count = sizeof(ch1_script) / sizeof(ch1_script[0]);
    int ch2_count = sizeof(ch2_script) / sizeof(ch2_script[0]);
    int ch3_count = sizeof(ch3_script) / sizeof(ch3_script[0]);
    int ch4_count = sizeof(ch4_script) / sizeof(ch4_script[0]);
    int ch5_count = sizeof(ch5_script) / sizeof(ch5_script[0]);
    
    copy_script(SCRIPT_CH1, ch1_script, ch1_count);
    copy_script(SCRIPT_CH2, ch2_script, ch2_count);
    copy_script(SCRIPT_CH3, ch3_script, ch3_count);
    copy_script(SCRIPT_CH4, ch4_script, ch4_count);
    copy_script(SCRIPT_CH5, ch5_script, ch5_count);
}

const char** script_get(int script_id, int *out_count) {
    static bool initialized = FALSE;
    if (!initialized) {
        script_data_init();
        initialized = TRUE;
    }
    
    if (script_id < 0 || script_id >= SCRIPT_COUNT) {
        *out_count = 0;
        return NULL;
    }
    *out_count = script_counts[script_id];
    return script_table[script_id];
}