import streamDeck from "@elgato/streamdeck";

import { HotkeyHold } from "./actions/hotkeyhold";


streamDeck.logger.setLevel("info");

// Register the actions.
streamDeck.actions.registerAction(new HotkeyHold());

// Finally, connect to the Stream Deck.
streamDeck.connect();
