import EmulationService from './service/EmulationService';
import { createContext } from 'react';

const EmulationContext = createContext<EmulationService | undefined>(undefined);

export default EmulationContext;
