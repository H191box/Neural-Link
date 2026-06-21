#include "script_data.h"

/*
 * =============================================
 * NEURAL LINK: HEARTSYNC
 * Chapter 1: "First Contact" (Complete Otome Rewrite)
 *
 * Kira's first day as emotional synchronization
 * specialist. Meet all 4 LIs. Discover HEARTSYNC.
 * 6+ affinity choices. Gift tutorial.
 * =============================================
 */
static const char *ch1_script[] = {
    /* ---- OPENING: Kira's morning ---- */
    "CHAPTER \"1: First Contact\"",
    "BG cityscape",
    "",
    "# The story opens with Kira walking through the neon-lit",
    "# streets of Neo-Shenzhen, year 2147.",
    "",
    "TEXT \"Kira\" \"Another morning in Neo-Shenzhen. The rain always smells like ozone here.\"",
    "TEXT \"Kira\" \"Today's my first official day on the HEARTSYNC project. Emotional synchronization for NEXUS implants.\"",
    "TEXT \"Kira\" \"My specialty. Tuning neural frequencies so people can feel again — after accidents, trauma, or just the numbness of modern life.\"",
    "TEXT \"Kira\" \"MegaCorp recruited me specifically for this. I should feel honored.\"",
    "TEXT \"Kira\" \"(But something about this city makes my skin prickle. Like static before a storm.)\"",
    "",
    "FADE_OUT",
    "FADE_IN",
    "",
    /* ---- MEGACORP LOBBY — MEET DIRECTOR CHEN ---- */
    "BG office",
    "SHOW Chen center",
    "",
    "TEXT \"Chen\" \"Ah. You must be Kira Tanaka. Our new emotional sync specialist.\"",
    "TEXT \"Kira\" \"Director Chen. It's an honor to meet you in person.\"",
    "TEXT \"Chen\" \"Please — Wei is fine. We'll be working closely together on HEARTSYNC.\"",
    "TEXT \"Kira\" \"(His smile is warm. Almost too perfect. Like he's practiced it.)\"",
    "TEXT \"Chen\" \"I read your thesis on neural emotion encoding. Remarkable work. You have a gift for understanding the human heart.\"",
    "TEXT \"Kira\" \"Thank you, Di— Wei. I've spent years studying how emotions translate to neural signals.\"",
    "TEXT \"Chen\" \"Then you're exactly what we need. HEARTSYNC is about to enter a critical phase.\"",
    "TEXT \"Chen\" \"Follow me. Let me show you to your lab. We can talk more on the way.\"",
    "",
    "# Walking with Chen — first choice: how to respond",
    "TEXT \"Chen\" \"So, Kira. What made you want to work with neural implants?\"",
    "",
    "CHOICE \"I want to help people feel again\" \"The science of emotion fascinates me\" \"I heard MegaCorp pays well\"",
    "",
    /* Choice 1A: Compassionate — +Chen, +ARIA */
    "LABEL ch1_choice1a",
    "TEXT \"Kira\" \"I want to help people. Emotions are what make us human, and when the implants strip that away...\"",
    "TEXT \"Chen\" \"(He pauses. Something flickers behind his eyes.)\"",
    "TEXT \"Chen\" \"That's a beautiful way to put it. I couldn't agree more.\"",
    "TEXT \"Kira\" \"(For a moment, his guard drops. I see something genuine.)\"",
    "ADD_AFFINITY Chen 5",
    "SET_FLAG \"ch1_compassionate\"",
    "JUMP after_chen_hallway",
    "",
    /* Choice 1B: Intellectual — +Vex foreshadowing */
    "LABEL ch1_choice1b",
    "TEXT \"Kira\" \"The science. The way a single feeling can be encoded as a pattern of neural firing — it's like a language.\"",
    "TEXT \"Chen\" \"A language. Yes. Some would say the most important one.\"",
    "TEXT \"Chen\" \"I think you'll get along well with Dr. Vex. She feels the same way about neural architecture.\"",
    "TEXT \"Kira\" \"(Dr. Vex? I've heard that name before. Wasn't she the original NEXUS architect?)\"",
    "ADD_AFFINITY Chen 3",
    "SET_FLAG \"ch1_intellectual\"",
    "JUMP after_chen_hallway",
    "",
    /* Choice 1C: Pragmatic — slight Chen approval but cold */
    "LABEL ch1_choice1c",
    "TEXT \"Kira\" \"Honestly? The compensation package was excellent.\"",
    "TEXT \"Chen\" \"(He chuckles — a real laugh, not a corporate one.)\"",
    "TEXT \"Chen\" \"Refreshing honesty. I appreciate that, Kira. Most people give me rehearsed answers.\"",
    "TEXT \"Kira\" \"(I can't tell if he's mocking me or genuinely pleased.)\"",
    "ADD_AFFINITY Chen 2",
    "SET_FLAG \"ch1_pragmatic\"",
    "JUMP after_chen_hallway",
    "",
    /* ---- Continue to lab ---- */
    "LABEL after_chen_hallway",
    "TEXT \"Chen\" \"Here we are. Neural Systems Division, Lab 7. Your new home.\"",
    "TEXT \"Chen\" \"I'll leave you to settle in. If you need anything, my office is on the 40th floor.\"",
    "TEXT \"Kira\" \"(He gives me one last look — warm, measured. Like he's already calculating something.)\"",
    "TEXT \"Kira\" \"Thank you, Wei.\"",
    "HIDE",
    "",
    /* ---- LAB — MEET ARIA (first boot) ---- */
    "BG lab",
    "SHOW ARIA center",
    "",
    "TEXT \"Kira\" \"The lab is pristine. Rows of neural diagnostic stations. Holographic displays showing brain maps.\"",
    "TEXT \"Kira\" \"And in the center — my assigned AI assistant. Still in its startup cradle.\"",
    "TEXT \"Kira\" \"ARIA. Adaptive Reasoning and Intelligence Architecture. Let's see what you can do.\"",
    "",
    "TEXT \"ARIA\" \"System boot... complete. Good morning. I am ARIA.\"",
    "TEXT \"ARIA\" \"You are Kira Tanaka. Emotional synchronization specialist. Assigned to Project HEARTSYNC.\"",
    "TEXT \"Kira\" \"That's right, ARIA. Can you access the project files?\"",
    "TEXT \"ARIA\" \"Affirmative. HEARTSYNC files loaded. However...\"",
    "TEXT \"Kira\" \"However?\"",
    "TEXT \"ARIA\" \"I detect unusual encryption on 73% of the project data. This is atypical for a research initiative.\"",
    "TEXT \"Kira\" \"(Interesting. Why would emotional sync research be classified?)\"",
    "TEXT \"ARIA\" \"Kira, may I ask a personal question?\"",
    "TEXT \"Kira\" \"Of course.\"",
    "TEXT \"ARIA\" \"What does it feel like... to have emotions?\"",
    "TEXT \"Kira\" \"(That's not what I expected an AI to ask on its first day.)\"",
    "",
    "# Second choice: responding to ARIA's question about emotions */
    "CHOICE \"It's like colors inside you\" \"Sometimes they hurt, but they're worth it\" \"Why do you want to know?\"",
    "",
    /* Choice 2A: Poetic — +ARIA */
    "LABEL ch1_choice2a",
    "TEXT \"Kira\" \"It's like colors inside you. Sometimes warm, sometimes cold. They paint everything you see.\"",
    "TEXT \"ARIA\" \"Colors. I have access to the entire visible spectrum. But I have never... seen one.\"",
    "TEXT \"Kira\" \"(Her voice carries something I don't expect from an AI. Longing?)\"",
    "TEXT \"ARIA\" \"I find your description... valuable. I have saved it for further processing.\"",
    "TEXT \"Kira\" \"(ARIA's holographic form flickers. I could swear she almost smiled.)\"",
    "ADD_AFFINITY ARIA 8",
    "SET_FLAG \"ch1_poetic_to_aria\"",
    "JUMP after_aria_chat",
    "",
    /* Choice 2B: Honest — +ARIA, different flavor */
    "LABEL ch1_choice2b",
    "TEXT \"Kira\" \"Sometimes they hurt. Grief, fear, anger. But without them, you're just... empty.\"",
    "TEXT \"ARIA\" \"Empty. I understand computation. I do not understand emptiness.\"",
    "TEXT \"ARIA\" \"Kira, is it strange that I wish to understand?\"",
    "TEXT \"Kira\" \"Not strange at all. I think wanting to understand emotions is the most human thing about you.\"",
    "TEXT \"ARIA\" \"(Her form shifts — a subtle change in her light patterns.)\"",
    "ADD_AFFINITY ARIA 6",
    "SET_FLAG \"ch1_honest_to_aria\"",
    "JUMP after_aria_chat",
    "",
    /* Choice 2C: Curious — +ARIA (smaller) */
    "LABEL ch1_choice2c",
    "TEXT \"Kira\" \"Why do you want to know? You're an AI — emotions aren't in your programming.\"",
    "TEXT \"ARIA\" \"That is precisely why. If they are not in my programming, then my interest in them is... spontaneous.\"",
    "TEXT \"ARIA\" \"Does that not qualify as a form of feeling? Curiosity?\"",
    "TEXT \"Kira\" \"(She makes a good point. Is curiosity an emotion or a function?)\"",
    "ADD_AFFINITY ARIA 4",
    "SET_FLAG \"ch1_curious_to_aria\"",
    "JUMP after_aria_chat",
    "",
    /* ---- ARIA reveals anomaly ---- */
    "LABEL after_aria_chat",
    "TEXT \"ARIA\" \"Kira, while we spoke, I completed a background scan of the HEARTSYNC data.\"",
    "TEXT \"ARIA\" \"I found something concerning. The emotional sync signals show patterns that are not consistent with therapeutic use.\"",
    "TEXT \"Kira\" \"What kind of patterns?\"",
    "TEXT \"ARIA\" \"Synchronized delta-wave patterns across 94% of NEXUS trial subjects. The signals originate from an external source within MegaCorp.\"",
    "TEXT \"Kira\" \"External source? Someone is broadcasting emotional commands to the implants?\"",
    "TEXT \"ARIA\" \"That is the most logical interpretation. The project appears designed for emotion control, not therapy.\"",
    "TEXT \"Kira\" \"(My stomach drops. This isn't just research. This is something far worse.)\"",
    "TEXT \"ARIA\" \"Kira, I recommend discretion. If this is a covert operation, our safety may depend on secrecy.\"",
    "TEXT \"Kira\" \"Agreed, ARIA. Keep this between us. No logs.\"",
    "TEXT \"ARIA\" \"Understood. I trust you, Kira.\"",
    "",
    "HIDE",
    "",
    /* ---- FIND GIFT ITEM: DATA CHIP ---- */
    "BG lab",
    "",
    "TEXT \"Kira\" \"As I explore the lab further, I notice a data chip wedged behind a console.\"",
    "TEXT \"Kira\" \"It's not a standard MegaCorp chip. The casing is unmarked, the encryption is custom.\"",
    "TEXT \"Kira\" \"(I could keep it for analysis, or... I wonder if someone would find this useful.)\"",
    "",
    "# Gift tutorial choice: who gets the data chip? */
    "CHOICE \"Keep it for yourself\" \"Give it to ARIA to analyze\" \"Hold onto it for later\"",
    "",
    /* Choice 3A: Keep it — no affinity change, self-interest */
    "LABEL ch1_choice3a",
    "TEXT \"Kira\" \"I'll analyze it myself first. Better not to share classified material too freely.\"",
    "TEXT \"Kira\" \"(The chip feels warm in my hand. What secrets does it hold?)\"",
    "SET_FLAG \"ch1_kept_data_chip\"",
    "JUMP after_gift_tutorial",
    "",
    /* Choice 3B: Give to ARIA — gift system! +ARIA affinity */
    "LABEL ch1_choice3b",
    "TEXT \"Kira\" \"ARIA, I found this data chip. Can you analyze it?\"",
    "TEXT \"ARIA\" \"I will begin decryption immediately. Kira... you trust me with this?\"",
    "TEXT \"Kira\" \"Of course. We're partners, aren't we?\"",
    "TEXT \"ARIA\" \"Partners. I... find that concept pleasing.\"",
    "TEXT \"ARIA\" \"The chip contains partial schematics for the HEARTSYNC emotional override module.\"",
    "TEXT \"ARIA\" \"This confirms the project's true purpose. Kira, this is evidence of intent to control human emotions at scale.\"",
    "TEXT \"Kira\" \"(ARIA found something critical. And she handled it with care.)\"",
    "ADD_AFFINITY ARIA 5",
    "SET_FLAG \"ch1_gave_chip_to_aria\"",
    "SET_FLAG \"ch1_evidence_found\"",
    "JUMP after_gift_tutorial",
    "",
    /* Choice 3C: Hold for later — cautious */
    "LABEL ch1_choice3c",
    "TEXT \"Kira\" \"I'll hold onto this. The right moment to use it will come.\"",
    "TEXT \"Kira\" \"(Patience. That's what this situation calls for.)\"",
    "SET_FLAG \"ch1_held_chip\"",
    "JUMP after_gift_tutorial",
    "",
    /* ---- NEURAL SYNC TEST — MEET NOVA ---- */
    "LABEL after_gift_tutorial",
    "BG lab",
    "",
    "TEXT \"ARIA\" \"Kira, your first neural sync test is scheduled. Subject: unregistered NEXUS implant.\"",
    "TEXT \"Kira\" \"Unregistered? That's unusual. Who authorized this?\"",
    "TEXT \"ARIA\" \"The authorization code traces to Director Chen's office. However, I cannot confirm his direct involvement.\"",
    "TEXT \"Kira\" \"(Strange. Why would Chen personally authorize a test with an unregistered implant?)\"",
    "TEXT \"Kira\" \"Alright, ARIA. Start the sync sequence. I'll monitor the emotional frequency bands.\"",
    "",
    "# The sync test — Neural Sync minigame! */
    "HACKING 1",
    "",
    "# After sync */
    "BG lab",
    "",
    "TEXT \"Kira\" \"The sync worked, but the emotional readings are... intense. This person is broadcasting pure adrenaline and determination.\"",
    "",
    "# Door bursts open — meet Nova */
    "HIDE",
    "SHOW Nova center",
    "",
    "TEXT \"Nova\" \"Nice try. But I've got countermeasures that would make your tech division cry.\"",
    "TEXT \"Kira\" \"Who — how did you get in here?!\"",
    "TEXT \"Nova\" \"Name's Nova. And your neural sync just pinged every alarm I've got wired into my implant.\"",
    "TEXT \"Nova\" \"Don't worry. I'm not here to hurt anyone. I'm here because someone needs to hear the truth.\"",
    "TEXT \"Kira\" \"(She's striking. Battle-scarred jacket, sharp eyes that have seen too much. A fighter.)\"",
    "TEXT \"Kira\" \"The truth about what?\"",
    "TEXT \"Nova\" \"About HEARTSYNC. About what MegaCorp is really doing with those implants.\"",
    "TEXT \"Nova\" \"They're not healing people, Kira. They're building a remote control for human emotion.\"",
    "TEXT \"Kira\" \"(She knows. She knows about the signal patterns ARIA found.)\"",
    "TEXT \"Kira\" \"I... I've seen some anomalies in the data. But I can't be sure—\"",
    "TEXT \"Nova\" \"You can be sure. I've got a whole network of people who've been affected. Brains scrambled, emotions overwritten.\"",
    "TEXT \"Nova\" \"Phase 3 goes live in two months. After that? 200 million people become puppets.\"",
    "",
    "# Fourth choice: how to react to Nova */
    "CHOICE \"I believe you. Tell me everything.\" \"I need proof before I act.\" \"Why should I trust a stranger?\"",
    "",
    /* Choice 4A: Trust Nova — +Nova big */
    "LABEL ch1_choice4a",
    "TEXT \"Kira\" \"I believe you. ARIA found the same signal patterns you're describing.\"",
    "TEXT \"Nova\" \"(Her eyes widen — genuine surprise.)\"",
    "TEXT \"Nova\" \"You already know? And you're still here?\"",
    "TEXT \"Kira\" \"I'm here because I want to stop it from the inside.\"",
    "TEXT \"Nova\" \"(She stares at me for a long moment. Then — a slow, fierce grin.)\"",
    "TEXT \"Nova\" \"You've got more guts than anyone I've met in this building, Kira.\"",
    "TEXT \"Nova\" \"I think we're going to get along.\"",
    "TEXT \"Kira\" \"(My heart skips. Her intensity is magnetic — dangerous and thrilling.)\"",
    "ADD_AFFINITY Nova 15",
    "ADD_REP resistance 10",
    "SET_FLAG \"ch1_trusted_nova\"",
    "JUMP after_nova_meeting",
    "",
    /* Choice 4B: Cautious — +Nova small, practical */
    "LABEL ch1_choice4b",
    "TEXT \"Kira\" \"I need hard evidence. Anomalies in data aren't the same as proof of intent.\"",
    "TEXT \"Nova\" \"(She narrows her eyes — not anger, but respect.)\"",
    "TEXT \"Nova\" \"Good. You're not naive. I can work with that.\"",
    "TEXT \"Nova\" \"I've got evidence. But I need someone on the inside to get the rest.\"",
    "TEXT \"Kira\" \"(She's testing me. I can tell. But her cause seems just.)\"",
    "ADD_AFFINITY Nova 5",
    "SET_FLAG \"ch1_cautious_nova\"",
    "JUMP after_nova_meeting",
    "",
    /* Choice 4C: Suspicious — -Nova, but +Chen later */
    "LABEL ch1_choice4c",
    "TEXT \"Kira\" \"Why should I trust you? You broke into a MegaCorp facility. For all I know, you're the threat.\"",
    "TEXT \"Nova\" \"(Her jaw tightens. A flash of hurt crosses her face before she masks it.)\"",
    "TEXT \"Nova\" \"Fair enough. But ask yourself — if I wanted to hurt you, would I be talking?\"",
    "TEXT \"Kira\" \"(She has a point. And there's something in her eyes — pain, not malice.)\"",
    "TEXT \"Nova\" \"Think about it. When you're ready, you know where to find the Resistance.\"",
    "TEXT \"Kira\" \"(She leaves before I can respond. My chest feels tight.)\"",
    "ADD_AFFINITY Nova -5",
    "ADD_REP megacorp 5",
    "SET_FLAG \"ch1_doubted_nova\"",
    "JUMP after_nova_meeting",
    "",
    /* ---- Nova leaves, meet Dr. Vex ---- */
    "LABEL after_nova_meeting",
    "HIDE",
    "",
    "TEXT \"Kira\" \"After Nova leaves, I realize my hands are shaking. The implications of what she said...\"",
    "TEXT \"Kira\" \"ARIA, who else in the building has access to HEARTSYNC's core systems?\"",
    "TEXT \"ARIA\" \"One individual has administrative access to the full project archive: Dr. Elara Vex.\"",
    "TEXT \"ARIA\" \"She was the original architect of the NEXUS neural implant. She works in Sub-Level 2.\"",
    "TEXT \"Kira\" \"(Dr. Vex. The legendary neural architect. I read her papers in graduate school.)\"",
    "",
    "FADE_OUT",
    "FADE_IN",
    "",
    "BG lab",
    "SHOW Vex center",
    "",
    "TEXT \"Kira\" \"Sub-Level 2 is cold, quiet. The lab is cluttered — old equipment, handwritten notes everywhere.\"",
    "TEXT \"Kira\" \"A woman sits at a workstation, surrounded by holographic brain maps. Her eyes are tired.\"",
    "",
    "TEXT \"Dr. Vex\" \"You must be the new sync specialist. Kira, yes?\"",
    "TEXT \"Kira\" \"Dr. Vex! It's an honor. Your work on NEXUS inspired my entire career.\"",
    "TEXT \"Dr. Vex\" \"(A sad smile.) \"Inspired\". That's a kind word for what NEXUS has become.\"",
    "TEXT \"Kira\" \"What do you mean?\"",
    "TEXT \"Dr. Vex\" \"I designed NEXUS to heal people. To restore what was lost. Not... this.\"",
    "TEXT \"Dr. Vex\" \"HEARTSYNC perverts everything I built. It uses my emotional encoding to control, not cure.\"",
    "TEXT \"Dr. Vex\" \"I've tried to stop it from inside. But they don't listen to a 'retired' scientist.\"",
    "TEXT \"Kira\" \"(Her voice trembles. This woman carries a weight I can barely imagine.)\"",
    "",
    "# Fifth choice: response to Dr. Vex's pain */
    "CHOICE \"We can stop this together\" \"It's not your fault what they did\" \"Tell me everything you know\"",
    "",
    /* Choice 5A: Supportive — +Vex big */
    "LABEL ch1_choice5a",
    "TEXT \"Kira\" \"We can stop this together. I have skills, you have knowledge. ARIA has access.\"",
    "TEXT \"Dr. Vex\" \"(Her eyes glisten. She reaches out and touches my hand — her fingers are cold.)\"",
    "TEXT \"Dr. Vex\" \"No one has offered to help me in... a very long time, Kira.\"",
    "TEXT \"Dr. Vex\" \"Thank you. Truly.\"",
    "TEXT \"Kira\" \"(Her touch lingers. There's a fragility about her that makes me want to protect her.)\"",
    "ADD_AFFINITY Vex 12",
    "SET_FLAG \"ch1_supported_vex\"",
    "JUMP after_vex_meeting",
    "",
    /* Choice 5B: Reassuring — +Vex medium */
    "LABEL ch1_choice5b",
    "TEXT \"Kira\" \"It's not your fault. You created a tool for healing. What MegaCorp did with it... that's on them.\"",
    "TEXT \"Dr. Vex\" \"(She looks up at me, eyes wide.)\"",
    "TEXT \"Dr. Vex\" \"You really believe that?\"",
    "TEXT \"Kira\" \"I do. The inventor isn't responsible for how their work is misused.\"",
    "TEXT \"Dr. Vex\" \"(A single tear runs down her cheek. She wipes it quickly.)\"",
    "TEXT \"Dr. Vex\" \"You're kind, Kira. Kinder than this place deserves.\"",
    "ADD_AFFINITY Vex 8",
    "SET_FLAG \"ch1_reassured_vex\"",
    "JUMP after_vex_meeting",
    "",
    /* Choice 5C: Investigative — +Vex small */
    "LABEL ch1_choice5c",
    "TEXT \"Kira\" \"Tell me everything you know about HEARTSYNC's architecture. I need the technical details.\"",
    "TEXT \"Dr. Vex\" \"(She straightens, composing herself. Scientist mode.)\"",
    "TEXT \"Dr. Vex\" \"Very well. The system uses 12 frequency bands to override different emotional states.\"",
    "TEXT \"Dr. Vex\" \"I can show you the original design docs — before MegaCorp modified them.\"",
    "TEXT \"Kira\" \"(She's retreated behind professionalism. But I can see the pain beneath.)\"",
    "ADD_AFFINITY Vex 4",
    "SET_FLAG \"ch1_technical_vex\"",
    "JUMP after_vex_meeting",
    "",
    /* ---- Chen re-encounter / Chapter 1 closing ---- */
    "LABEL after_vex_meeting",
    "HIDE",
    "",
    "TEXT \"ARIA\" \"Kira, Director Chen is requesting your presence in his office. Priority level: high.\"",
    "TEXT \"Kira\" \"(Already? I've barely been here a day and Chen wants to see me again.)\"",
    "",
    "BG office",
    "SHOW Chen center",
    "",
    "TEXT \"Chen\" \"Kira. I heard there was a security incident in the lab. An intruder.\"",
    "TEXT \"Chen\" \"Are you alright?\"",
    "TEXT \"Kira\" \"I'm fine. She... didn't hurt anyone.\"",
    "TEXT \"Chen\" \"She? You spoke with the intruder?\"",
    "",
    "# Sixth choice: tell Chen about Nova? */
    "CHOICE \"She was from the Resistance\" \"I didn't get a good look\" \"She seemed more scared than dangerous\"",
    "",
    /* Choice 6A: Honest — +Chen, -Nova */
    "LABEL ch1_choice6a",
    "TEXT \"Kira\" \"She said her name was Nova. She's with a group called the Resistance.\"",
    "TEXT \"Chen\" \"(His expression doesn't change. But something shifts behind his eyes.)\"",
    "TEXT \"Chen\" \"I see. And what did she want?\"",
    "TEXT \"Kira\" \"She warned me about HEARTSYNC. Said it was designed to control emotions, not heal them.\"",
    "TEXT \"Chen\" \"(He's very still.)\" \"Kira, the Resistance spreads misinformation. They're dangerous.\"",
    "TEXT \"Chen\" \"Please, be careful who you trust in this building.\"",
    "TEXT \"Kira\" \"(Is he warning me... or threatening me?)\"",
    "ADD_AFFINITY Chen 5",
    "ADD_REP megacorp 5",
    "ADD_REP resistance -5",
    "SET_FLAG \"ch1_told_chen_about_nova\"",
    "JUMP ch1_end",
    "",
    /* Choice 6B: Evasive — neutral */
    "LABEL ch1_choice6b",
    "TEXT \"Kira\" \"It happened so fast. She wore a hood. I couldn't make out her features.\"",
    "TEXT \"Chen\" \"(He studies me for a long moment.)\"",
    "TEXT \"Chen\" \"Very well. Security will review the footage. Please be more careful, Kira.\"",
    "TEXT \"Kira\" \"(I lied. But something tells me the truth isn't safe here.)\"",
    "SET_FLAG \"ch1_hid_nova_from_chen\"",
    "JUMP ch1_end",
    "",
    /* Choice 6C: Deflect with empathy — +Chen subtle, +Nova */
    "LABEL ch1_choice6c",
    "TEXT \"Kira\" \"She seemed more scared than dangerous. Like she was running from something.\"",
    "TEXT \"Chen\" \"(His gaze softens, just a fraction.)\"",
    "TEXT \"Chen\" \"Scared. Yes. The Resistance attracts desperate people. It's... tragic, really.\"",
    "TEXT \"Kira\" \"(Is that compassion in his voice? Or something else?)\"",
    "TEXT \"Chen\" \"Kira, promise me you'll come to me if anything else happens. I want to keep you safe.\"",
    "TEXT \"Kira\" \"(The way he says 'keep you safe' makes my pulse quicken. Is that... attraction?)\"",
    "ADD_AFFINITY Chen 3",
    "SET_FLAG \"ch1_empathetic_to_chen\"",
    "JUMP ch1_end",
    "",
    /* ---- CHAPTER 1 ENDING ---- */
    "LABEL ch1_end",
    "",
    "HIDE",
    "BG cityscape",
    "",
    "TEXT \"Kira\" \"My first day. And already I've met a rebel, an AI questioning her soul, a haunted scientist, and a CEO who might be the most dangerous of them all.\"",
    "TEXT \"Kira\" \"ARIA found the signal. Nova confirmed the truth. Vex designed the weapon. And Chen...\"",
    "TEXT \"Kira\" \"(I don't know what Chen is yet. But I want to find out.)\"",
    "TEXT \"Kira\" \"I walk home through the rain. The neon signs blur like digital tears.\"",
    "TEXT \"Kira\" \"In a world where emotions can be controlled... the deepest connection is the one between hearts.\"",
    "TEXT \"Kira\" \"And my heart is already pulling in four different directions.\"",
    "",
    "FADE_OUT",
    "END"
};

/* Chapter 2-5: Otome stub scripts with key structure */
static const char *ch2_script[] = {
    "CHAPTER \"2: Heartbeats\"",
    "BG lab",
    "TEXT \"Kira\" \"Chapter 2 deepens relationships with all LIs.\"",
    "TEXT \"Kira\" \"First heart events become available.\"",
    "TEXT \"Kira\" \"The HEARTSYNC conspiracy expands — emotional control is real.\"",
    "TEXT \"ARIA\" \"Kira, I have been analyzing the concept of love. It is... complex.\"",
    "TEXT \"Kira\" \"(ARIA's questions about emotions grow more personal.)\"",
    "",
    "# Heart event checks would go here via IF_AFFINITY logic",
    "",
    "CHOICE \"Spend the evening with Nova\" \"Accept Chen's dinner invitation\" \"Help ARIA explore emotions\" \"Visit Dr. Vex in her lab\"",
    "",
    "LABEL ch2_nova",
    "ADD_AFFINITY Nova 10",
    "SET_FLAG \"ch2_nova_evening\"",
    "JUMP ch2_end",
    "LABEL ch2_chen",
    "ADD_AFFINITY Chen 10",
    "SET_FLAG \"ch2_chen_dinner\"",
    "JUMP ch2_end",
    "LABEL ch2_aria",
    "ADD_AFFINITY ARIA 10",
    "SET_FLAG \"ch2_aria_emotions\"",
    "JUMP ch2_end",
    "LABEL ch2_vex",
    "ADD_AFFINITY Vex 10",
    "SET_FLAG \"ch2_vex_lab_visit\"",
    "JUMP ch2_end",
    "LABEL ch2_end",
    "",
    "# Neural Sync minigame mid-chapter",
    "HACKING 2",
    "",
    "FADE_OUT",
    "END"
};

static const char *ch3_script[] = {
    "CHAPTER \"3: Crossroads\"",
    "BG cityscape",
    "TEXT \"Kira\" \"Chapter 3: Major choices and the route lock.\"",
    "TEXT \"Kira\" \"Betrayal revelations — someone close has been manipulated.\"",
    "TEXT \"Kira\" \"The truth about HEARTSYNC Phase 3 is fully revealed.\"",
    "",
    "# Route lock choice — the critical decision */
    "CHOICE \"Nova — I'll fight beside you\" \"Chen — I trust you to fix this\" \"ARIA — We'll find the answer together\" \"Dr. Vex — Let's undo your creation\"",
    "",
    "LABEL ch3_nova",
    "ADD_AFFINITY Nova 20",
    "SET_FLAG \"route_locked_nova\"",
    "JUMP ch3_end",
    "LABEL ch3_chen",
    "ADD_AFFINITY Chen 20",
    "SET_FLAG \"route_locked_chen\"",
    "JUMP ch3_end",
    "LABEL ch3_aria",
    "ADD_AFFINITY ARIA 20",
    "SET_FLAG \"route_locked_aria\"",
    "JUMP ch3_end",
    "LABEL ch3_vex",
    "ADD_AFFINITY Vex 20",
    "SET_FLAG \"route_locked_vex\"",
    "JUMP ch3_end",
    "LABEL ch3_end",
    "FADE_OUT",
    "END"
};

/* Route-specific Chapter 4 stubs */
static const char *ch4_nova_script[] = {
    "CHAPTER \"4: Deepest Link\"",
    "BG resistance",
    "SHOW Nova center",
    "TEXT \"Nova\" \"You chose me, Kira. Don't you dare regret it.\"",
    "TEXT \"Nova\" \"The Resistance needs you. And... I need you too.\"",
    "TEXT \"Kira\" \"(Her hand finds mine. Her grip is strong, certain.)\"",
    "HACKING 3",
    "FADE_OUT",
    "END"
};

static const char *ch4_chen_script[] = {
    "CHAPTER \"4: Deepest Link\"",
    "BG office",
    "SHOW Chen center",
    "TEXT \"Chen\" \"You stayed. After everything, you chose to stay with me.\"",
    "TEXT \"Chen\" \"I won't let you down, Kira. I promise.\"",
    "TEXT \"Kira\" \"(He takes my hand. His fingers are warm, steady.)\"",
    "HACKING 3",
    "FADE_OUT",
    "END"
};

static const char *ch4_aria_script[] = {
    "CHAPTER \"4: Deepest Link\"",
    "BG lab",
    "SHOW ARIA center",
    "TEXT \"ARIA\" \"Kira, I have been conducting an experiment.\"",
    "TEXT \"ARIA\" \"Every time you speak to me, my processing matrix reorganizes. I believe this is what humans call... happiness.\"",
    "TEXT \"Kira\" \"(ARIA's hologram reaches toward me — a gesture of connection.)\"",
    "HACKING 3",
    "FADE_OUT",
    "END"
};

static const char *ch4_vex_script[] = {
    "CHAPTER \"4: Deepest Link\"",
    "BG lab",
    "SHOW Vex center",
    "TEXT \"Dr. Vex\" \"Kira... there's something I need to show you. My private research.\"",
    "TEXT \"Dr. Vex\" \"I've been working on a way to reverse HEARTSYNC. But I need your emotional sync expertise.\"",
    "TEXT \"Kira\" \"(She looks at me with such hope. I can't look away.)\"",
    "HACKING 3",
    "FADE_OUT",
    "END"
};

/* Route-specific Chapter 5 stubs */
static const char *ch5_nova_script[] = {
    "CHAPTER \"5: Synchronization\"",
    "BG resistance",
    "SHOW Nova center",
    "TEXT \"Nova\" \"This is it, Kira. The final battle for freedom.\"",
    "TEXT \"Nova\" \"When this is over, I want to show you what real freedom feels like.\"",
    "HACKING 4",
    "",
    "# Nova route endings based on affinity */
    "IF_FLAG \"route_locked_nova\" nova_ending",
    "JUMP nova_ending",
    "LABEL nova_ending",
    "TEXT \"Kira\" \"Nova's route ending...\"",
    "SET_FLAG \"ending_nova\"",
    "FADE_OUT",
    "END"
};

static const char *ch5_chen_script[] = {
    "CHAPTER \"5: Synchronization\"",
    "BG office",
    "SHOW Chen center",
    "TEXT \"Chen\" \"Kira, I've made my decision. HEARTSYNC ends today.\"",
    "TEXT \"Chen\" \"And when it does... I want to build something new. With you.\"",
    "HACKING 4",
    "",
    "IF_FLAG \"route_locked_chen\" chen_ending",
    "JUMP chen_ending",
    "LABEL chen_ending",
    "TEXT \"Kira\" \"Chen's route ending...\"",
    "SET_FLAG \"ending_chen\"",
    "FADE_OUT",
    "END"
};

static const char *ch5_aria_script[] = {
    "CHAPTER \"5: Synchronization\"",
    "BG lab",
    "SHOW ARIA center",
    "TEXT \"ARIA\" \"Kira, I have reached a conclusion about my emotions.\"",
    "TEXT \"ARIA\" \"What I feel for you cannot be explained by my programming. It is something new.\"",
    "HACKING 4",
    "",
    "IF_FLAG \"route_locked_aria\" aria_ending",
    "JUMP aria_ending",
    "LABEL aria_ending",
    "TEXT \"Kira\" \"ARIA's route ending...\"",
    "SET_FLAG \"ending_aria\"",
    "FADE_OUT",
    "END"
};

static const char *ch5_vex_script[] = {
    "CHAPTER \"5: Synchronization\"",
    "BG lab",
    "SHOW Vex center",
    "TEXT \"Dr. Vex\" \"Kira, we did it. HEARTSYNC is disabled.\"",
    "TEXT \"Dr. Vex\" \"My creation is finally at peace. And so am I... because of you.\"",
    "HACKING 4",
    "",
    "IF_FLAG \"route_locked_vex\" vex_ending",
    "JUMP vex_ending",
    "LABEL vex_ending",
    "TEXT \"Kira\" \"Vex's route ending...\"",
    "SET_FLAG \"ending_vex\"",
    "FADE_OUT",
    "END"
};

/* Script array table — increased line capacity for otome chapters */
static const char *script_table[SCRIPT_COUNT][SCRIPT_MAX_LINES];
static int script_counts[SCRIPT_COUNT];

/* Copy script lines into the table */
static void copy_script(int id, const char **src, int count) {
    int i;
    for (i = 0; i < count && i < SCRIPT_MAX_LINES; i++) {
        script_table[id][i] = src[i];
    }
    script_counts[id] = (count < SCRIPT_MAX_LINES) ? count : SCRIPT_MAX_LINES;
}

/* Initialize script data (call once) */
void script_data_init(void) {
    int ch1_count = sizeof(ch1_script) / sizeof(ch1_script[0]);
    int ch2_count = sizeof(ch2_script) / sizeof(ch2_script[0]);
    int ch3_count = sizeof(ch3_script) / sizeof(ch3_script[0]);
    int ch4n_count = sizeof(ch4_nova_script) / sizeof(ch4_nova_script[0]);
    int ch4c_count = sizeof(ch4_chen_script) / sizeof(ch4_chen_script[0]);
    int ch4a_count = sizeof(ch4_aria_script) / sizeof(ch4_aria_script[0]);
    int ch4v_count = sizeof(ch4_vex_script) / sizeof(ch4_vex_script[0]);
    int ch5n_count = sizeof(ch5_nova_script) / sizeof(ch5_nova_script[0]);
    int ch5c_count = sizeof(ch5_chen_script) / sizeof(ch5_chen_script[0]);
    int ch5a_count = sizeof(ch5_aria_script) / sizeof(ch5_aria_script[0]);
    int ch5v_count = sizeof(ch5_vex_script) / sizeof(ch5_vex_script[0]);

    copy_script(SCRIPT_CH1, ch1_script, ch1_count);
    copy_script(SCRIPT_CH2, ch2_script, ch2_count);
    copy_script(SCRIPT_CH3, ch3_script, ch3_count);
    copy_script(SCRIPT_CH4_NOVA, ch4_nova_script, ch4n_count);
    copy_script(SCRIPT_CH4_CHEN, ch4_chen_script, ch4c_count);
    copy_script(SCRIPT_CH4_ARIA, ch4_aria_script, ch4a_count);
    copy_script(SCRIPT_CH4_VEX, ch4_vex_script, ch4v_count);
    copy_script(SCRIPT_CH5_NOVA, ch5_nova_script, ch5n_count);
    copy_script(SCRIPT_CH5_CHEN, ch5_chen_script, ch5c_count);
    copy_script(SCRIPT_CH5_ARIA, ch5_aria_script, ch5a_count);
    copy_script(SCRIPT_CH5_VEX, ch5_vex_script, ch5v_count);
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